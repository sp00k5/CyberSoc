# Splunk: Setting up a SOC Lab
> [TryHackMe Link](https://tryhackme.com/room/splunklab)

We would create an account on splunk.com and go to this Splunk Enterprise [download link](https://www.splunk.com/en_us/download/splunk-enterprise.html?locale=en_us) to select the installation package for the latest version


## Splunk Forwarders
Splunk has two primnary forwarders that can be utilised

### Heavy Forwarders

Heavy forwarders are used when we need to apply a filter, analyze or make changes to the logs at the source before forwarding it to the destination. 

### Universal Forwarders

It is a lightweight agent that gets installed on the target host, and its main purpose is to get the logs and send them to the Splunk instance or another forwarder without applying any filters or indexing. It has to be downloaded separately and has to be enabled before use.
> Universal forwarders can be downloaded from the official [Splunk website](https://www.splunk.com/en_us/download/universal-forwarder.html?locale=en_us).


# Linux Lab

- Install Splunk on Ubuntu Server
- Install and integrate Universal Forwarder
- Collecting Logs from important logs sources/files like syslog, auth.log, audited, etc

## Install Steps
1. `ls` to the directory with the installation `.tgz` file
2. `tar xvzf splunk_installer.tgz`
3. `mv splunk /opt/`
4. `cd /opt/splunk/bin/`
5. `./splunk start --accept-license`


### Installation of Universal Forwarder

1. `ls` to the directory with the installation `.tgz` file
2. `tar xvzf splunkforwarder.tgz`
3. `mv splunkforwarder /opt/`
4. `cd /opt/splunkforwader`
5. `./bin/splunk start --accept-license`

> by default, Splunk forwarder runs on port 8089 

### Configuring Forwarder on Linux

Need to configure the host end to send the data and configure Splunk so that it knows from where it is receiving the data

- Splunk Configuration
  1. Log into Splunk, Go to **Settings** -> **Forward and receiving** tab as shown below
  2. Click on **Configure receiving** and then proceed by configuring a new receiving port (*we want to receive data from the Linux endpoint*)
  3. By default, the Splunk instance receives data from the forwarder on the port `9997`
- Index Creation
  1. Create an index that will store all the receiving data (If no index is specified an a default index, called the main index, will be used)
  2. Indexes tab contains all the indexes created by the user or by default Shows metadata about the indexes like **Size**, **Event Count**, **Home Path**, **Status**
  3. Click the New Index button, fill out the form, and click Save to create the index
- Configuring Forwarder
  1. `cd /opt/splunkforwarder/bin` 
  2. `./splunk add forward-server 10.10.121.63:9997`
  3. `./splunk add monitor /var/log/syslog -index linux_host`
  4. `cat /opt/splunkforwarder/etc/apps/search/local/inputs.conf`

- Logger is a built-in command line tool to create test logs added to the syslog file (`logger "coffely-has-the-best-coffee-in-town" && tail -1 /var/log/syslog`)


# Windows Lab

- Install Splunk on Windows Machine
- Install and Integrate the Universal Forwarder
- Integrating and monitoring Coffely.THM's weblogs
- Integrating Windows Event Logs

# Generic CLI Commands

These commands are run from the /opt/splunk/ directory. It is important to note that we can use the same commands on different platforms.

| Commands | Description | 
| -------- | ----------- | 
| `splunk start` | Command starts all the necessary Splunk processes and enables the server to accept incoming data (*if the server is already running, this command will have no effect*) |
| `splunk stop` | Used to stop the Splunk server and stops all the running Splunk processes and disables the server from accepting incoming data |
| `splunk restart` | Used to restart the Splunk server and stops all the running Splunk processes and then starts them again. This is useful when changes have been made to the Splunk configuration files or when the server needs to be restarted. | 
| `splunk status` | Used to check the status of the Splunk server and will display information about the current state of the server, including whether it is running or not, and any errors that may be occurring | 
| `splunk add oneshot` | Used to add a single event to the Splunk index. This is useful for testing purposes or for adding individual events that may not be part of a larger data stream | 
| `splunk search` | Used to search for data in the Splunk index and can be used to search for specific events, as well as to perform more complex searches using Splunk's search language. | 
| `splunk help` | See all the commands splunk has access to | 