# Splunk: Basics
> [TryHackMe Link](https://tryhackme.com/room/splunk101)

Splunk is a platform SIEM for collecting, storing, and analysing machine data such as network and machine logs in real-time.. It provides various tools for analysing data, including search, correlation, and visualisation.

Splunk has three main components: 

## Splunk Forwarder 
A lightweight agent installed on the endpoint intended to be monitored, and its main task is to collect the data and send it to the Splunk instance. Examples including: 
- Windows machine generating Windows Event Logs, PowerShell, and [Sysmon](https://learn.microsoft.com/en-us/sysinternals/downloads/sysmon) data.
- Linux host generating host-centric logs.
- Database generating DB connection requests, responses, and errors.


## Splunk Indexer 
Splunk Indexer plays the main role in processing the data it receives from forwarders. It takes the data, normalizes it into field-value pairs, determines the datatype of the data, and stores them as events. 

## Search Head
The place within the Search & Reporting App where users can search the indexed logs. When the user searches for a term the request is sent to the indexer and the relevant events are returned in the form of field-value pairs.
- Search Head also provides the ability to transform the results into presentable tables, visualizations like pie-chart, bar-chart and column-chart

The main page is comprised of: 
- Splunk Bar shows:
    - Messages - system-level messages 
    - Settings - configure the Splunk instance 
    - Activity - review the progress of jobs 
    - Help - miscellaneous information such as tutorials 
    - Find - a search feature
- App Panel - List the apps installed for the Splunk instance. The default app for every Splunk installation is **Search & Reporting** 
- Explore Splunk - Contains quick links to add data to the Splunk instance, add new Splunk apps, and access the Splunk documentation
- Home Dashboard

Splunk can ingest any data where it is processed and transformed into a series of individual events. Data sources are grouped into categories as listed here in the [documentation](https://help.splunk.com/en/splunk-enterprise/get-started/search-tutorial/10.0/part-2-uploading-the-tutorial-data/about-uploading-data) 
