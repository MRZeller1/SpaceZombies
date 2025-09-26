Overview
Space Zombies is a fast-paced arcade shooter where you battle waves of zombies and bugs using a dynamic pathfinding system. Enemies use a grid-based vector field to intelligently navigate around obstacles and converge on your position. Survive as long as possible while racking up points!
Features

Intelligent Enemy AI: Enemies use distance-based pathfinding to navigate around obstacles
Dynamic Grid System: Real-time path updates as you move through the map
Multiple Enemy Types:

Zombies: Slower but deal heavy damage
Bugs: Fast-moving swarm enemies with lower damage


Sprint Mechanic: Temporary speed boost with cooldown
Score System: Track your survival time
Camera Follow System: Smooth camera tracking centered on the player
Collision System: Physics-based collision with enemies, bullets, and environment

Controls

Arrow Keys: Move player (Up/Down/Left/Right)
W: Fire weapons
R: Reload
F: Sprint (30 second cooldown, 2.5 second duration)
A: Change weapon (currently single weapon available)

Requirements

CMake 3.20 or higher
C++17 compatible compiler
Raylib library (included in lib/)

Project Structure
SpaceZombies/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── character.h         # Base character class
│   ├── player.h           # Player implementation
│   ├── npc.h              # Base NPC class
│   ├── zombie.h           # Zombie enemy
│   ├── bug.h              # Bug enemy
│   ├── grid.h             # Pathfinding grid system
│   ├── gridNode.h         # Individual grid cell
│   ├── collisionMap.h     # Collision detection system
│   ├── bullet.h           # Projectile system
│   ├── weapon.h           # Base weapon class
│   ├── pistols.h          # Dual pistols weapon
│   ├── gameobject.h       # Static objects/obstacles
│   └── game.h             # Main game loop
├── src/                    # Implementation files
├── resources/              # Game assets (textures)
└── lib/                    # Raylib library files
Game Mechanics
Pathfinding System

The game uses a grid-based vector field for enemy navigation
Each grid cell stores distance to the player and optimal direction
Updates occur in a radius around the player for performance
Enemies smoothly turn toward the player using momentum-based movement

Combat System

Pistols fire bullets at enemies
Bullets have limited lifetime and travel in a straight line
Collision detection between bullets, enemies, and static objects
Different damage values for player and enemy types

Enemy Spawning

Zombies spawn at random unoccupied positions away from the player
Bugs spawn in 3x3 grids at multiple locations
New waves spawn when all enemies are defeated
Enemy count: 100 zombies, 540 bugs per wave

Health & Damage

Player Health: 100 HP
Zombie Damage: 25 HP per hit
Bug Damage: 5 HP per hit
Bullet Damage: 10 HP
Player speed increases 2x while sprinting

Technical Details
Grid System

Grid Size: 3025x3025 pixels
Cell Size: 25x25 pixels
121 x 121 total cells
Dynamic distance calculation using BFS algorithm

Performance Optimizations

Partial grid updates (50 cell radius around player)
Efficient collision detection with spatial hashing
Object pooling for bullets
Smart texture loading and unloading

Known Issues & Future Improvements

Additional weapon types (rifle, flamethrower) are stubbed but not implemented
Player rotation animation not fully implemented
Consider adding sound effects and music
Power-ups and weapon upgrades could enhance gameplay
Minimap display for better spatial awareness

Credits
Built with Raylib - A simple and easy-to-use library to enjoy videogames programming
License
[Add your license information here]
