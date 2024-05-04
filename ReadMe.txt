<<--------------------------------------------------------------------------------Physics_Project_01---------------------------------------------------------------------------------------------------------->>

# Developers :
	- This project was done as a group, by Max Sherwin(1128059) and Keerthivasan Kalaivanan(1190826)

# About Project :
	- The project reads the scene contents( i.e the model properties, physics properties, light properties, etc) from the json file located in "Physics_Project_01->CollisionProject->SceneDescription.json". The project loads the models and the physics properties as mentioned in the human readable file and displays the physics related scene. 

	- In the scene we have created a 3d room with a floor and 3 walls. The 4th wall and the roof is open so that the camera can view the simulation happening. There will be 10 spheres colliding with each other and the environment. 
	
	- If the spheres fall off the ground into the open 3d space..after a while they'll be reset and will join back in the simulation.
	
	- There are also 6 cubes that are "not axis aligned" and "dynamically moving" in the centre of the simulation. So the spheres colliding will alos interact with the cubes at different angles and pace.
	
	- The spheres when they collide with eachother will "change it's color" randomly. Initially the spheres begin as white in color and change color as they collide with other spheres. During reset they are turned back to white again.

	- We've also added a button to add additional force to the spheres in the direction that they're already moving. Button for wireframe mode as well.

	- We have added a plane for the ground which has multiple triangles on it. 
	
	- The json file holds the scene description and changing the json file will change the scene. But the format of the json file should be the same to load the scene. 

## How to build and run : 
	- The solution "PhysicsProject_01.sln" has to be built(either in debug or release platform mode) and then once succeeded(probably will!), make the "CollisionProject" project as the startup project and now it'll be able to run for some visual treat. 
	
### Controls :
	- Default Camera Controls : 
		- W, A, S, D, Q, E = Move camera Front, Back, Left, Right, Up and Down respectively. 
		- Mouse movement = Camera can be rotated using the mouse
		- Spacebar = Spacebar pauses and resumes the mouse movement(By default the cam rotation is in a pause, so when the scene loads, press Spacebar to activate camera rotation through mouse)
		-  1, 2, 3, 4 = These numbers keys can be pressed to showcase 4 different angles of the scene.

	- Extra Controls : There a few controls that enable the user to manipulate the models in the scene :
		- CTRL + W, A, S, D, Q, E = To move the models around, Front, Back, Left, Right, Up and Down respectively.
		- CTRL + Z, X = Shift to previous model or next model respectively.
		- CTRL + B = Turn on/off wireframe mode for the selected model.
		- CTRL + SPACEBAR = Adds a small amount of force or a push to the sphere making it move faster towards the direction it is already travelling to.

#### Limitations to the project : 
	- Although the mouse camera rotation is a bit effective than the keypad, it's still not perfected. There are some flaws like, when the mouse movement is paused and resuming the mouse movement after moving the cam position, the rotation seems to be irregular at first and then gets used to the new perspective. 
	- We've added a button to add extra external force to the moving sphere but since there are 10 spheres in simulation it is not easy to see to which sphere the force is added to. 
	- Sometimes the single collision response glitches and the ball gets stuck inside the cubes or the walls. 
	- We tried adding the multiple collision response but the response wasnt as expected. The sphere kept moving irregularly. So we have commented the code for the multiple collision response which is in "PhysicsEngine->cPhysics.cpp->PlaneCollisionResponse()". Please let us know in the feedback if there are any corrections we can do to the formula or anything else to make it work. Hopefully we'll figure it out before the next Physics project :)