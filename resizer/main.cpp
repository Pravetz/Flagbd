#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


int main()
{
	std::cout<<"Image resizing tool by Pravetz, written for Flagbd."<<std::endl;
	int x, y;
	std::string filename;
	std::getline(std::cin, filename);
	
	sf::Texture loaded_texture;
	if(!loaded_texture.loadFromFile(filename)){
			return EXIT_FAILURE;
		}
	
	sf::Sprite loaded_sprite(loaded_texture);
	
	std::cout<<"New (X, Y) values: ";
	std::cin>>x>>y;
	
	sf::RenderWindow app(sf::VideoMode(x, y, 32), "Result ("+std::to_string(x)+";"+std::to_string(y)+")");
	sf::RenderTexture resized_texture;
	
	resized_texture.clear(sf::Color::Black);
	
	resized_texture.create(x,y);
	
	
	///////////////////////////// RESIZE ///////////////////////////////
	sf::Vector2u size_texture = loaded_texture.getSize();
	
	float scale_x = (float) x / size_texture.x;
	float scale_y = (float) y / size_texture.y;
	
	loaded_sprite.setScale(scale_x,scale_y);
	
	
	while(app.isOpen()){
			sf::Event event;
		while (app.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					app.close();
			}
			
			resized_texture.draw(loaded_sprite);
			resized_texture.display();
			
			sf::Sprite result(resized_texture.getTexture());
			
			app.clear(sf::Color::Black);
			app.draw(result);
			app.display();
		}
		
		resized_texture.getTexture().copyToImage().saveToFile(filename);
	return EXIT_SUCCESS;
}
