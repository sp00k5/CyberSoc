# Governance & Regulation
> [TryHackMe Link](https://tryhackme.com/room/cybergovernanceregulation)


- **Governance**: Managing and directing an organisation or system to achieve its objectives and ensure compliance with laws, regulations, and standards.
- **Regulation**: A rule or law enforced by a governing body to ensure compliance and protect against harm.
- **Compliance**: The state of adhering to laws, regulations, and standards that apply to an organisation or system.

## Information Security Governance
Information security governance represents an organisation's established structure, policies, methods, and guidelines designed to guarantee the privacy, reliability, and accessibility of its information assets.  It is essential for risk management, safeguarding confidential data and adhering to pertinent regulations. 
- **Strategy**: Developing and implementing a comprehensive information security strategy that aligns with the organisation's overall business objectives.
- **Policies and procedures**: Preparing policies and procedures that govern the use and protection of information assets.
- **Risk management**: Conduct risk assessments to identify potential threats to the organisation's information assets and implement risk mitigation measures.
- **Performance measurement**: Establishing metrics and key performance indicators (KPIs) to measure the effectiveness of the information security governance program.
- **Compliance**: Ensuring compliance with relevant regulations and industry best practices.

## Information Security Regulation
Refers to legal and regulatory frameworks that govern the use and protection of information assets. Regulations are designed to protect sensitive data from unauthorized access, theft, and misuse. Compliance with regulations is typically mandatory and enforced by government agencies or other regulatory bodies (e.g GDPR, PCI DSS, PIPEDA). The benefits of these are:
- Robust Security Posture
- Increased Stakeholder Confidence
- Regulatory Compliance
- Business objective allignment
- Informed decision-making
- Competitive advantage

> Note: Laws and regulations operate the security governance and regulatory ecosystem. Providing a structured framework for establishing minimum compliance standards. Offering clear and concise rules, they reduce ambiguity and provide a common language for organisations to measure their security posture and ensure regulatory compliance

## Information Security Frameworks
The information security framework provides a comprehensive set of documents that outline the organisation's approach to information security and governs how security is implemented, managed, and enforced within the organisation. This mainly includes:
- **Policies**: A formal statement that outlines an organisation's goals, principles, and guidelines for achieving specific objectives.
- **Standards**: A document establishing specific requirements or specifications for a particular process, product, or service.
- **Guidelines**: A document that provides recommendations and best practices (non-mandatory) for achieving specific goals or objectives.
- **Procedures**: Set of specific steps for undertaking a particular task or process.
- **Baselines**: A set of minimum security standards or requirements that an organisation or system must meet.

### Developing Governance Documents

<details>
<summary>Here are some generalised steps used to develop policies, standards, guidelines:</summary>

   - **Identify the scope and purpose**:  Determine what the document will cover and why it is needed. A baseline might be required to establish a minimum level of security for all systems.
   - **Research and review**: Research relevant laws, regulations, industry standards, and best practices to ensure your document is comprehensive and up-to-date.
   - **Draft the document**: Develop an outline and start drafting the document, following best practices. Ensure the document is specific, actionable, and aligned with the organisation's goals and values.
   - **Review and approval**: Have the document reviewed by stakeholders, such as subject matter experts, legal and compliance teams, and senior management. Incorporate their feedback and ensure the document aligns with organisational goals and values. Obtain final approval from appropriate stakeholders.
   - **Implementation and communication**: Communicate the document to all relevant employees and stakeholders, and ensure they understand their roles and responsibilities in implementing it. Develop training and awareness programs to ensure the document is understood and followed.
   - **Review and update**: Periodically review and update the document to ensure it remains relevant and practical. Monitor compliance and adjust the document based on feedback and changes in the threat landscape or regulatory environment.
</details>

> Note: There are numerous factors upon which we decide which standard framework of baseline checklist should be used; these include regulatory requirements primarily related to the particular geographical areas, scope, objectives, available resources, and many more.
    
## Governance and Risk Compliance (GRC) framework
Focuses on steering the organisation's overall governance, enterprise risk management, and compliance in an integrated manner.
- Governance Component: Involves guiding an organisation by setting its direction through information security strategy (it is the rules processes and policies to steer the org). Along with establishing appropriate monitoring methods to measure its performance and assess the outcomes.
- Risk Management Component: Involves identifying, assessing, and prioritising risks to the organisation and implementing controls and mitigation strategies to manage those risks effectively (continuously evaluating and refining the risk management program)
- Compliance Component: Ensuring that the organisation meets its legal, regulatory, and industry obligations and that its activities align with its policies and procedures (e.g compliance programs, conducting regular audits, assessments)

### Develop a GRC Program
| Steps                | Details | Example |
| -------------------------- | ----------------------------------------- | -------------- |
|  Define the scope and objectives            |  Determining the scope of the GRC program and defining its goals             | Objective might be to reduce cyber risks to 50% in the next 12 months while maintaining the trust of its customers |
| Conduct a risk assessment          |  Organisation identifies and assesses its cyber risks          | Perform a risk assessment to reveal vulnerable points or outdated software. Then prioritise these risks and develop a risk management strategy |
| Develop policies and procedures         | Policies and procedures are developed to guide cyber security practices within the organisation            | Establish a password policy to ensure the usage of strong passwords |
| Establish governance processes          |  Governance processes ensure the GRC program is effectively managed and controlled          | Establish a security steering committee that meets regularly to review security risks and make decisions about security investments and prioritie |
| Implement controls          |  Technical and non-technical controls are implemented to mitigate risks identified in risk assessment           | Implement firewalls, IPS, IDS, SIEM to prevent external attacks and impart employee training to improve security awareness and reduce the risk of human error |
|  Monitor and measure performance         |  Processes are established to monitor and measure the effectiveness of the GRC program           | Track metrics and compliance with security policies. This information is used to identify areas for improvement and adjust the program as needed |
| Continuously improve          | The GRC program is constantly reviewed and improved based on performance metrics, changing risk profiles, and stakeholder feedback          | Conduct a post-incident analysis to identify the root cause and make changes to prevent a similar incident from happening again |

## Privacy of PII
Privacy regulations help ensure individuals' personal information is handled and stored responsibly and ethically. They also help to establish trust, protect personal information, and maintain regulatory compliance.


### General Data Protection Regulation (GDPR)
a data protection law implemented by the EU in May 2018 to protect personal data. 
- **Personal data**  - Any data associated with an individual that can be utilised to identify them either directly or indirectly". Key points of the law include the following:
   - **Prior approval** must be obtained before collecting any personal data.
   - Personal data should be kept to a **minimum** and only collected when necessary.
   - **Adequate measures** are to be adopted to protect stored personal data.

<p align="center" width="75%">
    <img width="40%" src="https://tryhackme-images.s3.amazonaws.com/user-uploads/62a7685ca6e7ce005d3f3afe/room-content/772ddef796e37d2c2f82c1bb9b240a36.png">
</p>

> Note: The law applies to all business entities that conduct business in the European Union (EU) and collect/store/process the personal data of EU residents and are required to comply. Moreover, this also includes penalties and fines based on non-compliance in the following two tiers:
  - Tier 1: More severe violations, including unintended data collection, sharing data with third parties without consent, etc. Maximum penalty amounting to 4% of the organisation's revenue or 20 million euros (whichever is higher).
  - Tier 2: Less severe violations, including data breach notifications, cyber policies, etc. The maximum fine for Tier 2 is 2% of the organisation's revenue or 10 million euros (whichever is higher).

### Payment Card Industry Data Security Standard  
PCI DSS is focused on maintaining secure card transactions and protecting against data theft and fraud. It is widely used by businesses, primarily online, for card-based transactions and it was established by major credit card brands (Visa, MasterCard & American Express). 

## NIST 800-53
A publication titled "Security and Privacy Controls for Information Systems and Organisations", that provides a catalogue of security controls to protect the CIA triad of information systems. Offers a comprehensive set of security and privacy controls that organisations can use to safeguard their operations, assets, personnel, and other organisations from various threats and risks. These include intentional attacks, unintentional errors, natural disasters, infrastructure failures, foreign intelligence activity, and privacy concerns. Address 4 specific security concerns
- Administrative Control
- Technical Controls 
- Physical Controls
- Strategic Controls

Program Management control mandates establishing, implementing, and monitoring organisation-wide programs for information security and privacy while safeguarding the processed, stored, or transmitted through systems

<p align="center" width="75%">
    <img width="40%" src="https://tryhackme-images.s3.amazonaws.com/user-uploads/62a7685ca6e7ce005d3f3afe/room-content/43092cc2b85ee7ce4395ce6d7afd842f.png">
</p>

### Compliance Best Practices 
- Discovery and Classify: Recognise and catalogue their data assets, information systems, and associated threats including understanding data flows, system dependencies, and potential vulnerabilities
- Map: Control families must be mapped to the identified assets and hazards
- Manage: Creating a governance structure allocating duties, and outlining precise controls implementation and maintenance procedures
- Monitor: All measures must be regularly monitored and evaluated to ensure compliance
    
## NIST 800-63B
Set of guidelines to help organisations establish effective digital identity practices. The primary focus is on authenticating and verifying the identities of individuals who access digital services, systems, and networks
    
## Information Security (IS) management
Protects information assets from unauthorised access, use, disclosure, interruption, alteration, and destruction. It involves risk assessment and identification, security controls and procedures development, incident response planning, and security awareness training
 
**Compliance**: Observing information security-related legal, regulatory, contractual, and industry-specific standards

### ISO/IEC 27001
ISO 27001 is an internationally recognised standard for requirements to plan, develop, run, and update an organisation's Information Security Management System (ISMS).
<details>
<summary>Here are some generalised steps used to develop policies, standards, guidelines:</summary>

   - **Scope**: This specifies the ISMS's boundaries, including the covered assets and processes
   - **Information security policy**: A high-level document defining an organisation's information security approach
   - **Risk assessment**: Involves identifying and evaluating the risks to the CIA of the organisation's information.
   - **Risk treatment**: Involves selecting and implementing controls to reduce the identified risks to an acceptable level.
   - **Statement of Applicability (SoA)**: Document specifies which controls from the standard are applicable and which are not.
   - **Internal audit**: This involves conducting periodic audits of the ISMS to ensure that it is operating effectively.
   - **Management review**: Review the performance of ISMS at regular intervals.
</details>

An ISMS built on the ISO 27001 standard requires careful design and execution. It entails:
- Evaluating the organisation's security procedures, detecting gaps, and conducting a thorough risk assessment
- Leadership support and resource allocation
- Regular monitoring, measurement, and continual development 

## Service Organisation Control 2
[SOC 2](https://soc2.co.uk/) was developed by AICPA as a compliance/auditing framework. Focuses on assessing the efficacy of a company's data security based on the CIA triad and privacy.
- Essential for service providers interacting with client data or offering solutions that process, store, or transmit sensitive data

### SOC 2 Cardinals 
1. Independent auditors conduct SOC 2 audits to determine that security controls meet the relevant criteria.
2. The audit may cover physical security, network security, data encryption, backup and recovery, and employee training and awareness.
3. Information can be shared with customers and other stakeholders to ensure the company takes appropriate measures to protect its data and systems.

### Planning SOC 2 Audit
<details>
<summary>Steps can be taken by an organisation’s management team before and during an audit:</summary>

   - **Determine the scope**: Includes systems, processes, or locations that are relevant to the security and privacy of customer data.
   - **Choose a suitable auditor**: Select a qualified auditor with experience conducting SOC 2 audits (consider reputation, experience, availability)
   - **Plan the audit**: Work with the auditor to plan the audit, including the audit timeline, the scope of the audit, and the audit criteria.
  - **Prepare for the audit**: Review your security and privacy controls, policies, and procedures. Identify any gaps or deficiencies and develop a plan to address them.
  - **Conduct the audit**: The auditor will review your controls and conduct testing to assess their effectiveness. The audit may include interviews with key personnel, documentation reviews, and controls tests.
  - **Receive the audit report**: The report may include a description of your controls, any deficiencies or gaps identified, and recommendations for improvement.
</details>

> **Note:** Primary purpose of the SOC 2 audit is to ensure that third-party service providers store and process sensitive information securely. 
