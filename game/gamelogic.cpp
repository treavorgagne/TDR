#include "gamelogic.hpp"

using namespace sf;

// all these are used right now, so i might just delete the file

Player localCollision(Player p, GameMap map, std::vector<Map_Element> walls, Vector2f currPosition){

		// local collision detection between player, boundary, and walls
		if (p.box.getPosition().x < 0) p.box.setPosition(0, p.box.getPosition().y);
		if (p.box.getPosition().x > map.mapWidth) p.box.setPosition((float) map.mapWidth, p.box.getPosition().y);
		if (p.box.getPosition().y < 0) p.box.setPosition(p.box.getPosition().x, 0);
		if (p.box.getPosition().y > map.mapHeight) p.box.setPosition(p.box.getPosition().x, (float) map.mapHeight);
		for (size_t j = 0; j < walls.size(); j++) {
			if (p.box.getGlobalBounds().intersects(walls[j].wall.getGlobalBounds())) {
				p.box.setPosition(currPosition);
			}
		}

		return p;

}

Player playerMovement(Player p, GameEngine eng){

		//Client I/O manager
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			p.box.move(-eng.movementSpeed, 0.0f);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 1, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			p.box.move(0.0f, -eng.movementSpeed);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 3, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			p.box.move(eng.movementSpeed, 0.0f);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 2, p.textureSize.x, p.textureSize.y));
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			p.box.move(0.0f, eng.movementSpeed);
			p.box.setTextureRect(IntRect(p.textureSize.x * (p.walk++ % 4), p.textureSize.y * 0, p.textureSize.x, p.textureSize.y));
		}

		return p;
}
