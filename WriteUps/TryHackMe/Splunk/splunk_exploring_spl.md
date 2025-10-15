# Splunk: Exploring SPL
> [TryHackMe Link](https://tryhackme.com/room/splunkexploringspl)

## Search & Reporting App
Default interface used to search and analyze the data on the Splunk Home page. Functionality includes using: 
- Search Head
- Time Duration
- Search History
- Field Sidebar 

| SideBar | Description |
| --------| ----------- |
| **Selected Fields** | Splunk extracts the default fields like source, sourcetype, and host, which appear in each event, and places them under the selected fields column. We can select other fields that seem essential and add them to the list |
| **Interesting Fields** | Pulls all the interesting fields it finds and displays them in the left panel to further explore | 
| A**lpha-numeric fields 'α'** | This alpha symbol shows that the field contains text values | 
| **Numeric fields '#'** | This symbol shows that this field contains numerical values |
| **Count** | The number against each field shows the number of events captured in that timeframe | 

## SPL
Comprises of multiple functions, operators and commands that are used together to form a simple to complex search and get the desired results from the ingested logs

### Search Field Operators
Filter, remove, and narrow down the search result based on the given criteria. Common field operators are Comparison operators, wildcards, and boolean operators.

- **Comparison Operators**: These operators are used to compare the values against the fields (=,!=,<,>,>=)
- **Boolean Operators**: These operators are used in searching/filtering and narrowing down results (NOT, OR, AND)
- **Wild Card**: Used to match the characters in the strings (*)
    - `status=fail*` returns results like status=failed / status=failure

## Filters  

- **Fields** - Used to add or remove mentioned fields from the search results. To remove the field, minus sign ( - ) is used before the fieldname and plus ( + ) is used before the fields which we want to display. (`fields + HostName - EventID`)
- **Search** - Used to search for the raw text while using the chaining command (`| search "text"` )
- **DeDup** - Used to remove duplicate fields from the search results. (`dedup fieldName`)
- **Rename** - Used us to change the name of the field in the search results (`rename User as Employees`)

## Sorting

- **Table** - Each event has multiple fields, and not every field is important to display. Used to create a table with selective fields as columns. (`table <field_name>`)
- **Head** - Returns the first 10 events if no number is specified (`head x`)
- **Tail** - Returns the last 10 events if no number is specified (`tail x`)
- **Sort** - Used to order the fields in ascending or descending order (`sort <filed_name>` *will sort the result in Ascending order*)
- **Reverse** - Reverses the order of the events (reverse)

## Transformational Commands
These change the result into a data structure from the field-value pairs. These commands simply transform specific values for each event into numerical values which can easily be utilized for statistical purposes or turn the results into visualizations. 
- **Top** - command returns frequent values for the top 10 events (`top limit=3 EventID`)
- **Rare** - command does the opposite of top command as it returns the least frequent values or bottom 10 results. (`rare limit=3 EventID`)
- **Highlight** -  highlight command shows the results in raw events mode with fields highlighted (`highlight <field_name>`)
- **Stats** - supports various stats commands that help in calculating statistics on the values (`stats avg(field_name)` or max,min,sum,count)
    - Average: Used to calculate the average of the given field
    - Max: Used to return the maximum value from the specific field
    - Min: Used to return the minimum value from the specific field
    - Sum: Used to return the sum of the fields in a specific value
    - Count: Used to return the number of data occurrences

There are transforming commands that are used to present the data in table or visualization form
- **Chart** -  Used to transform the data into tables or visualizations (`chart count by field_name`) where the syntax after chart takes a function
- **TimeChart** - Used to return the time series chart covering the field following the function mentioned (`timechart count by Image`)