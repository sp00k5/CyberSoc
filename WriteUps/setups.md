   
# Setups   
List of ways to setup stuff   
   
## Tor   

### Setup   
- [torsocks](https://gist.github.com/valyakuttan/ce4afb62288120cd5ecef0fde4ea63c4) < --- tor setup   
- [FREE PROXY](https://spys.one/en/) <-- sus website (Indian)   
   
### Usage   
> everything in shell will be through tor  ⤵   

```
source torsocks on / off
```

> firefox will be through tor ⤵   
```
proxychains -f ./proxychains.conf firefox google.com
```
   
## Podman   

Open-source fork of docker   
### Flags   
> --rm <- delete pod once exited   
> -it <- interactive pod   
> -network=podman <- listen to podman network   
> -d <- run as background deamon   
> --privileged <-run as root   
> -name NAME  <- create pod with name NAME   

### Usage   
> list all pods ⤵   

```
sudo podman ps --all

```
> delete a pod ⤵   

```
sudo podman rm hash

```
> create interactive alpine pod with network  ⤵   

```
sudo podman run --rm -it --network=podman --privileged alpine

```
> copy FILE/FOLDER to podman shell directory ⤵   

```
sudo podman cp FILE  vigilant_beaver:/home/

```
> Attach back to background pod   

```
sudo podman attach HASH

```
   
## Flask with Nginx   
 
> PreRequisites   

```
sudo apt-get install python3 python3-pip python3-dev build-essential libssl-dev libffi-dev python3-setuptools python3-venv nginx -y 
systemctl start nginx
systemctl enable nginx

```
> Creating a Virtual Environment   

```
cd ~/project
python3 -m venv .
source venv/bin/activate
pip install wheel
pip install gunicorn flask
pip install -r requirements.txt (if needed)

```
> Flask Required Lines   

```
app.py: app.run(host='0.0.0.0')
wsgi.py: app.run()
Check: gunicorn --bind 0.0.0.0:5000 wsgi:app
deactivate

```
> Setup   

```
nano /etc/systemd/system/name.service

[Unit]
Description=Gunicorn instance to serve Flask
After=network.target
[Service]
User=root
Group=www-data
WorkingDirectory=/project
Environment="PATH=/project/NAME/bin"
ExecStart=/root/project/NAME/bin/gunicorn --bind 0.0.0.0:5000 wsgi:app
[Install]
WantedBy=multi-user.target

chown -R root:www-data /directory
chmod -R 775 /directory
systemctl daemon-reload
systemctl start name
systemctl enable name
systemctl status name

```
> Configure NGINX   

```
nano /etc/nginx/conf.d/name.conf

server {
    listen 80;
    server_name TLD;
    location / {
        include proxy_params;
        proxy_pass  http://127.0.0.1:5000;
    }
}

nginx -t
systemctl restart nginx

```
   
## Flashing Windows over Ethernet   

### Two machines:   
**Source** - SSD you want to copy
**Target** - SSD you want to write to   
- install dstat   
- sudo apt install libarchive-zip-perl --no-install-recommends (gets crc32)   
   
### Transfer:   
- Load up both devices onto Linux over Live USB boot   
- plug them into a switch with ethernet   
- ls /dev/sd\*   
- Both Machines: mount -> check which is the drive that you will be reading / writing to (sda in this case)   
- Source: dd if=/dev/sda bs=1M status=progress \| nc -lp 31337   
- Target: nc source\_ip 31337 \| dd of=/dev/sda bs=1M status=progress   
- Should now see that they are connecting and data is being copied   
- Once done Ctrl + C   
   
### Checks:   
- Source: cat /sys/class/block/sda/size (important number)   
- Source: crc32 /dev/sda   
- Target: dd if=/dev/sda bs=512 count=number \| crc32 /dev/stdin   
- If they are the same congrats   
   
   
## Updating Java on Raspbian   

> get JRE versions for arm32 https://adoptopenjdk.net/releases.html - sudo mkdir   

```
/usr/lib/jvm/java17 - sudo mv OpenJDK17U-jre_arm_linux_hotspot_17.0.4.1_1.tar.gz 

/usr/lib/jvm/java17/ - cd /usr/lib/jvm/java17 - sudo tar -xf OpenJDK17U-

jre_arm_linux_hotspot_17.0.4.1_1.tar.gz - sudo update-alternatives --install /usr/bin/java 

java /usr/lib/jvm/java17/jdk-17.0.4.1+1-jre/bin/java 1131 - sudo update-alternatives --config 

java - choose the version

```

## Recover files from .vdi file
1. `cp FILE.vdi ~`
2. `cp VBoxDDU.dll  VBoxManage.exe  VBoxRT.dll ~` (need these from C:/Program Files/Oracle/VirtualBox/)
3. `./vdi2raw.sh Fedors.vdi` (https://gist.github.com/GFlorent/41b36e56f061ca9ad6dc30a61d9cfae3)
   - offset selection for btrfs to access user files
5. `sudo mount /dev/loop0 /mnt/vdi`
6. Once Done
   -  `sudo umount /mnt/vdi && sudo losetup -d /dev/loop0`
