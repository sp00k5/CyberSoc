# Active Directory Basics
> [TryHackMe Link](https://tryhackme.com/room/winadbasics)

## Windows domain
A group of users and computers under the administration of a given business aimed to centralise the administration of common components of a Windows computer network in a single repository called **Active Directory** (AD). The server that runs the Active Directory services is known as a **Domain Controller** (DC). This can include:
- File / Database server
- Desktops / Workstations

The advantages of this configuration are: 
- **Centralised identity management**: All users across the network can be configured from Active Directory with minimum effort.
- **Managing security policies**: You can configure security policies directly from Active Directory and apply them to users and computers across the network as needed

## Active Directory
The core of any Windows Domain is the **Active Directory Domain Service** (ADDS). This service acts as a catalogue that holds the information of all of the "objects" that exist on your network (*users*, *groups*, *machines*, *printers*, *shares* etc.)

### Users 
Users are one of the objects known as **security principals** (*an object that can act upon resources in the network*), meaning that they can be authenticated by the domain and can be assigned privileges over resources like files or printers. They can represent: 
- **People**: Persons in your organisation that need to access the network (i.e employees)
- **Services**: Every single service requires a user to run, but service users are different from regular users as they will only have the privileges needed to run their specific service. (i.e IIS, MSSQL)

### Machines
Every computer that joins the Active Directory domain will have a corresponding machine object created. Machines are also considered "security principals" and are assigned an account just as any regular user. This account has somewhat limited rights within the domain itself. 

The machine accounts themselves are local administrators on the assigned computer, they are generally not supposed to be accessed by anyone except the computer itself, but as with any other account, if you have the password, you can use it to log in.
- Machine account names are the computer's name followed by the $ sign

> **Note**: Machine Account passwords are automatically rotated out and are generally comprised of 120 random characters.

### Security Groups
Security groups are also considered security principals and, therefore, can have privileges over resources on the network. Groups can have both users, machines or other groups as members. Several groups are created by [default in a domain](https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/manage/understand-security-groups#default-active-directory-security-groups) that can be used to grant specific privileges to users: 

| Security Group | Description |
| -------------- | ----------- |
| Domain Admins  |	Users of this group have administrative privileges over the entire domain. By default, they can administer any computer on the domain, including the DCs | 
| Server Operators |	Users in this group can administer Domain Controllers. They cannot change any administrative group memberships | 
| Backup Operators |	Users in this group are allowed to access any file, ignoring their permissions. They are used to perform backups of data on computers |
| Account Operators | 	Users in this group can create or modify other accounts in the domain |
| Domain Users |	Includes all existing user accounts in the domain | 
| Domain Computers |	Includes all existing computers in the domain| 
|Domain Controllers |	Includes all existing DCs on the domain |

## AD Users and Computers
To configure users, groups or machines in Active Directory, we need to log in to the Domain Controller and run **Active Directory Users and Computers**

Objects are organised in **Organizational Units** (OUs) which are container objects that allow you to classify users and machines. OUs are mainly used to define sets of users with similar policing requirements (*a user can only be a part of a single OU at a time*). It is typical to see the OUs mimic the business' structure, as it allows for efficiently deploying baseline policies that apply to entire departments

Default containers exist from Windows like:
- **Builtin**: Contains default groups available to any Windows host.
- **Computers**: Any machine joining the network will be put here by default. You can move them if needed.
- **Domain Controllers**: Default that contains the DCs in your network.
- **Users**: Default users and groups that apply to a domain-wide context.
- **Managed Service Accounts**: Holds accounts used by services in your Windows domain.

### Security Groups vs OUs
- **OUs**: For applying policies to users and computers, which include specific configurations that pertain to sets of users depending on their particular role in the enterprise.
- **Security Groups**: Used to grant permissions over resources. For example, you will use groups if you want to allow some users to access a shared folder or network printer. A user can be a part of many groups, which is needed to grant access to multiple resources.

## Managing Users in AD

### Delegation
Give specific users some control over some OUs and allow users specific privileges to perform advanced tasks on OUs without needing a Domain Administrator to step in (e.g *IT support the privileges to reset other low-privilege users' passwords*).
- `Set-ADAccountPassword <USER> -Reset -NewPassword (Read-Host -AsSecureString -Prompt 'New Password') -Verbose` (reset a user's password)
- `Set-ADUser -ChangePasswordAtLogon $true -Identity <USER> -Verbose` (force them to change it when logging in)

## Managing Computers in AD

By default, all the machines that join a domain (except for the DCs) will be put in the container called "Computers". Segregating devices into the following groups is a good idea: 
1. **Workstations** - The device users will use to do their work or normal browsing activities (should never have a privileged user signed into them)
2. **Servers** - Generally used to provide services to users or other servers
3. **Domain Controllers** - Allow you to manage the Active Directory Domain (deemed the most sensitive devices within the network as they contain hashed passwords for all user accounts within the environment)

## Group Policies
Windows manages policies (*different configurations and security baselines to users depending on their department*) through **Group Policy Objects** (GPO). GPOs are a collection of settings that can be applied to OUs and contain policies aimed at either users or computers, allowing you to set a baseline on specific machines and identities.

To configure GPOs, you use the **Group Policy Management** tool. Any GPO will apply to the linked OU and any sub-OUs under it.

A GPO is made up of: 
- **Scope**: Where the GPO is linked in the AD
  - **Security Filtering** applied to GPOs so that they are only applied to specific users/computers under an OU (default will apply to the Authenticated Users group, which includes all users/PCs)
- **Details** - General details about the GPO
- **Settings** - Includes the actual contents of the GPO and what specific configurations it applies split by what applies to users and computers

### GPO distribution
GPOs are distributed to the network via a network share called `SYSVOL`, which is stored in the DC. All users in a domain typically have access to this share over the network to sync their GPOs periodically. The SYSVOL share points by default to the `C:\Windows\SYSVOL\sysvol\` directory on each of the DCs in our network.

> **Note**: It can take up to 2 hours for a GPO changes to catch up on computers. To force any particular computer to sync its GPOs immediately, you can run the following command on the desired computer `gpupdate /force`

## Authentication Methods
Whenever a user tries to authenticate to a service using domain credentials, the service will need to ask the Domain Controller to verify if they are correct. Two protocols can be used for network authentication in windows domains:
- **Kerberos**: Used by any recent version of Windows. This is the default protocol in any recent domain.
- **NetNTLM**: Legacy authentication protocol kept for compatibility purposes.

### Kerberos Authentication

Users who log into a service using Kerberos will be assigned tickets (i.e proof of previous authentication). The following process happens:
1. User sends their **username** plain and a **timestamp** encrypted using a key derived from their password to the **Key Distribution Center** (KDC), a service usually installed on the Domain Controller in charge of creating Kerberos tickets on the network.
   1. The KDC will create and send back a **Ticket Granting Ticket** (TGT), which will allow the user to request additional tickets to access specific services. The TGT is encrypted using the **krbtgt account's password hash**
   2. A **Session Key** is given to the user encrypted using the ysers hash, needed to generate the following requests
   3. The encrypted TGT includes a copy of the Session Key as part of its contents, and has no need to store the Session Key as it can recover a copy by decrypting the TGT if needed.
2. When a user wants to connect to a service on the network (e.g share, website, database) they will use their **TGT** along with their **username and a timestamp both encrypted using the Session Key** and include a **Service Principal Name** (SPN) (which indicates the service and server name they intend to access) to ask the KDC for a **Ticket Granting Service** (TGS)
   1. TGS are tickets that allow connection only to the specific service they were created for
   2. The KDC will send a TGS along with a **Service Session Key** encrypted using the users Session Key, which will be needed to authenticate to the service. The TGS is encrypted using a key derived from the **Service Owner Hash**
   3. The Service Owner is the user or machine account that the service runs under
3. The TGS can then be sent to the desired service to authenticate and establish a connection. 
   1. The service will use its configured account's password hash to decrypt the and validate the Service Session Key.
   2. The user also sends across their Username and Timestampe both encrypted with the Service Session Key

### NetNTLM Authentication
NetNTLM works using a challenge-response mechanism.
1. Client sends an authentication request to the server they want to access.
2. Server generates a random number and sends it as a challenge to the client.
3. Client combines their NTLM password hash with the challenge (and other known data) to generate a response to the challenge and sends it back to the server for verification.
4. Server forwards the challenge and the response to the Domain Controller for verification.
5. Domain controller uses the challenge to recalculate the response and compares it to the original response sent by the client. If they both match, the client is authenticated; otherwise, access is denied. The authentication result is sent back to the server.
6. Server forwards the authentication result to the client. 

> **Note**: The described process applies when using a domain account. If a local account is used, the server can verify the response to the challenge itself without requiring interaction with the domain controller since it has the Spassword hash stored locally on its SAM.

## Larger Management

### Trees
Active Directory supports integrating multiple domains so that you can partition your network into units that can be managed independently. Two domains that share the same namespace (thm.local in our example), those domains can be joined into a Tree.
- Domain Administrators of each branch will have complete control over their respective DCs, but not other branches' DCs.
- **Enterprise Admins** group will grant a user administrative privileges over all of an enterprise's domains. 
  - Each domain would still have its Domain Admins with administrator privileges over their single domains and the Enterprise Admins who can control everything in the enterprise.

### Forests
Managed domains can also be configured in different namespaces. The union of several trees with different namespaces into the same network is known as a forest (i.e if you acquired another company you would have different domains and IT departments)

### Trust Relationships
If a user at THM UK needs to access a shared file in one of HTB ASIA servers. Domains arranged in trees and forests are joined together by trust relationships allowing this to happen. 
- **One-way trust relationship**: If Domain AAA trusts Domain BBB, means a user on BBB can be authorised to access resources on AAA
  - The direction of the one-way trust relationship is contrary to that of the access direction
- **Two-way trust relationships**: Made to allow both domains to mutually authorise users from the other (this is the default action of joining several domains under a tree or forest)

> **Note**: Once a trust relationship is established, you have the chance to authorise users across different domains (it is not default access)