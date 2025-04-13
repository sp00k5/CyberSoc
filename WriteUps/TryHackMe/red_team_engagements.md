# Red Team Engagements
> [TryHackMe Link](https://tryhackme.com/room/redteamengagements)

Engagements can be categorized between a general internal/network penetration test or a focused adversary emulation. A focused adversary emulation will define a specific APT or group to emulate within an engagement. 

A client's scope will typically define what you cannot do or target; it can also include what you can do or target. While client objectives can be discussed and determined along with the providing team, a scope should only be set by the client.

## Rules of Engagement 
A legally binding outline of the client objectives and scope with further details of engagement expectations between both parties.
- This document often acts as the general contract between the two parties; an external contract or other NDAs (Non-Disclosure Agreement) can also be used

Standard Sections witnessed include: 

| Section Name                                  | Section Details                                                                              |
| --------------------------------------------- | -------------------------------------------------------------------------------------------- |
| Executive Summary                             | Overarching summary of all contents and authorization within RoE document                    |
| Purpose                                       | Defines why the RoE document is used                                                         |
| References                                    | Any references used throughout the RoE document (HIPAA, ISO, etc.)                           |
| Scope                                         | Statement of the agreement to restrictions and guidelines                                    |
| Definitions                                   | Definitions of technical terms used throughout the RoE document                              |
| Rules of Engagement and Support Agreement     | Defines obligations of both parties and general technical expectations of engagement conduct |
| Provisions                                    | Define exceptions and additional information from the Rules of Engagement                    |
| Requirements, Restrictions, and Authority     | Define specific expectations of the red team cell                                            |
| Ground Rules                                  | Define limitations of the red team cell's interactions                                       |
| Resolution of Issues/Points of Contact        | Contains all essential personnel involved in an engagement                                   |
| Authorization                                 | Statement of authorization for the engagement                                                |
| Approval                                      | Signatures from both parties approving all subsections of the preceding document             |
| Appendix                                      | Any further information from preceding subsections                                           |

An example for a RoE can be found at [redteam.guide](https://redteam.guide/docs/templates/roe_template/)

> Note: The format and wording of the RoE are critical since it is a legally binding contract and sets clear expectations.

## Campaign planning
Uses the information acquired and planned from the client objectives and RoE and applies it to various plans and documents to identify how and what the red team will do. 
- **Engagement Plan**: An overarching description of technical requirements of the red team.
- **Operations Plan**: An expansion of the Engagement Plan. Goes further into specifics of each detail.
- **Mission Plan**: The exact commands to run and execution time of the engagement.
- **Remediation Plan**: Defines how the engagement will proceed after the campaign is finished.

### Engagement Plan
An overarching description of technical requirements of the red team. Engagement documentation is an extension of campaign planning where ideas and thoughts of campaign planning are officially documented. (*CONOPS* / *Resource and Personnel Requirements* / *Timelines*)

### Concept of Operation (CONOPS) 
Details a high-level overview of the proceedings of an engagement; the document will serve as a business/client reference and a reference for the red team to build off of and extend to further campaign plans.
- The CONOPS document should be written from a semi-technical summary perspective, assuming the target audience/reader has zero to minimal technical knowledge.

<details>
<summary><b>Outline of critical components that should be included in a CONOPS</b></summary>

   - Client Name
   - Service Provider
   - Timeframe
   - General Objectives/Phases
   - Other Training Objectives (Exfiltration)
   - High-Level Tools/Techniques planned to be used
   - Threat group to emulate (if any)
</details>

### Resource Plan
Written as bulleted lists of subsections it includes timelines and information required for the red team to be successful
- any resource requirements: personnel, hardware, cloud requirements
- detailing a brief overview of dates, knowledge required (optional), resource requirements

| Section Name                                  | Section Details                                                                              |
| --------------------------------------------- | -------------------------------------------------------------------------------------------- |
| Header              | - Personnel writing<br> - Dates <br> - Customer |
| Engagement Dates    | - Reconnaissance Dates<br> - Initial Compromise Dates<br> - Post-Exploitation and Persistence Dates<br> - Misc. Dates  |
| Knowledge Required (optional) | - Reconnaissance<br> - Initial Compromise <br> - Post-Exploitation                         |
| Resource Requirements   |  - Personnel<br> - Hardware<br> - Cloud<br> - Misc.                         |

 
### Operations Plan
A flexible document(s) that provides specific details of the engagement and actions occurring extending the CONOPS (*Operators* / *Known Information* / *Responsibilities*)
- Should follow a similar writing scheme using bulleted lists and small sub-sections

<details>
<summary><b>Outline of of example subsections within the operations plan</b></summary>

- Header
   - Personnel writing
   - Dates
   - Customer
- Halting/stopping conditions (can be placed in ROE depending on depth)
- Required/assigned personnel
- Specific TTPs and attacks planned
- Communications plan
- Rules of Engagement (optional)

</details>

> **Note**: The communications plan should summarize how the red cell will communicate with other cells and the client overall. (vectr.io / Email / Slack)

### Mission Plan
The mission plan is a cell-specific document that details the exact actions to be completed by operators including the exact commands to run and execution time of the engagement. (*Commands to run* / *Time Objectives* / *Responsible Operator*)
- Objectives
- Operators
- Exploits/Attacks
- Targets (users/machines/objectives)
- Execution plan variations

### Remediation Plan
Defines how the engagement will proceed after the campaign is finished. (*Report* / *Remediation consultation*)
- Report: Summary of engagement details and report of findings
- Remediation/consultation: How will the client remediate findings? It can be included in the report or discussed in a meeting between the client and the red team

