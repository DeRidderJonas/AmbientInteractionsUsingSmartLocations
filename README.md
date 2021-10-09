# AmbientInteractionsUsingSmartLocations
Research project regarding ambient interactions (NPC's interacting in the background) using Smart locations

This project represents a basic version of how to implement Smart Locations.
This project was made using the Elite Framework (Credits to Digital Arts And Entertainment) and the [Box2D](https://box2d.org/) framework.
It was inspired by this [research paper](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter35_Ambient_Interactions_Improving_Believability_by_Leveraging_Rule-Based_AI.pdf)

*When I'm talking about Agents, they can refer to NPCs or a player. This means any Character that can interact with anything*

![Smart Location Agents Talking](/Assets/AgentsTalking.gif)  
This Smart Location represents 2 agents talking and a third agent joining afterwards. We'll go over how this works later.

# Smart Locations
## What is a Smart Location?
A Smart Location is a place inside your game world where NPCs or the players can interact with certain objects and each other.
In this project, we only focus on NPCs interacting with each other. But it was built so these extra interactions could be easily added.
An example of a Smart Location could be: A table with a couple of chairs. The agents can interact with these chairs by sitting on them, or they could put some object on the table.

In this project, a Smart Location is represented by a green circle.
<img src="/Assets/SmartLocationRepresentation.png" alt="Smart Location representation" width="250" height="250">

## How to use them
In order for the Smart Location to be able to do something, it has to know about the [agents](#Agent) within reach and the [scripts](#Script) it can execute.
These scripts are provided to the Smart Location and takes ownership of them.
The script is always checking the agents to see which ones are within reach and comparing them to the [Roles](#Role) needed in the scripts.
If the Smart Location finds a match, it will start the script using the matching agents. 
The Smart Location is also the one to end the script when the end condition for the script has been met.

## Implementation
[SmartLocation.h](/code/source/projects/App_AmbientInteractions/SmartLocation.h)  
`void NotifyAgents(std::vector<NpcAgent*>& agents);` uses the agents given to it (all agents available for Smart Locations, in this project: all agents) and checks wether they are available.
`void StartScript();` and `void UpdateScripts(float deltaTime);` will start and Update or End scripts respectively.

# Agent
Agents can represent humans or animals. They are represented by a circle with an arrow in it (Default in Box2D).  
![Agent representation](/Assets/AgentRepresentation.png)  
The color of the agent body varies depending on wether the agent in within the reach of a Smart Location and the script it participates in.
Color | Meaning
----- | -------
Red | Default value
Green | Withing range of Smart Location and available for a script
Blue | Used in script [AgentsTalking](/code/source/projects/App_AmbientInteractions/Scripts/AgentsTalking.h) as a human talking
Purple | Used in script [DancingHumans](/code/source/projects/App_AmbientInteractions/Scripts/DancingHumans.h) as a dancer
Black | Used in script [DancingHumans](/code/source/projects/App_AmbientInteractions/Scripts/DancingHumans.h) as a part of the audience

Agents can only assume certain [roles](#Role).

## Implementation
[NpcAgent.h](/code/source/projects/App_AmbientInteractions/NpcAgent.h)  
The NpcAgents are pretty basic. They can use simple steering behaviors and hold some data to check wether they can assume a given role and wether they are available for scripts.

# Role
Every script has roles. These are what decide which agents and how many of them can join the script and dictate what to do during the script.
They also dictate wether an agents can join after a script has already started or what to do after the script has ended.

An agent can have these roles:
* Any
	* Human
		* Citizen
		* Tourist
		* Jogger
	* Animal
		* Dog
		* Bird

## Implementation
[Role.h](/code/source/projects/App_AmbientInteractions/Role.h)  
The roles are used during Role Allocation to check wether the script can start so it has data members for this and data members for when the script is executing.

# Script
The script dicates what conditions have to be met in order to start and end the script and what to do in between.
Each script uses a personal blackboard for whatever information it might need.

At this moment there are only 2 scripts: [AgentsTalking](/code/source/projects/App_AmbientInteractions/Scripts/AgentsTalking.h) and [DancingHumans](/code/source/projects/App_AmbientInteractions/Scripts/DancingHumans.h).
The names are quite self explanatory. The project was build to make it as easy as possible to add new scripts.

## Agents Talking
![AgentsTalking](/Assets/AgentsTalking.gif)  
2 agents enter the Smart Location. After a couple of seconds, a third agent joins. After the agents are done talking, they all leave.  

## Dancing Humans
![DancingHumans](/Assets/DancingHumans.gif)  
3 agents enter the Smart Location. They assume start positions and start dancing (due to lack of animations, they just run around in a circle). When more agents enter the Smart Location, they are considered the audience and they will stand and watch.  

In both gifs above, agents will continue to walk into the Smart Location even after all spots have been filled. In this case, they will be ignored by the script and act as if they weren't there.

## How to add a new script
1. Make a new class and derive from the [Script](/code/source/projects/App_AmbientInteractions/Script.h) class.
1. Decide what roles it should have (Add these in the constructor)
1. Override the necessary methods. These will always include `virtual void Start(Elite::Blackboard* pBlackboard) override;` and `virtual bool Update(float deltaTime) override;`. It's important to call the Script version of these functions as well because they are responsible for initialization.
1. Add it to a Smart Location

# Future
This was obviously a small project. Big games using the concept of Smart Locations need to make decisions on where to store data from the agents, wether they're available, compatible, etc... In the [research paper](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter35_Ambient_Interactions_Improving_Believability_by_Leveraging_Rule-Based_AI.pdf) mentioned before, they recommend a Tuple space where all of this data can be stored and accessed by the Smart Locations.

This project was also limited to a certain amount of agents and the scripts were made according to that amount. Also, it did not matter how they entered the Smart Location and the Smart Locations were always accessible. If larger amounts of agents are needed or if the Smart Location was placed in a location than "" was not (easily) accesible for agents, an "NPC spawner" is required. This spawner should provide the needed agents for the scripts.
