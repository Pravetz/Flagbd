#include <TGUI/TGUI.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>


int w=600;
int h=400;

int heraldry=20;			// CHANGE THESE VARIABLES AFTER ADDIG NEW
int adh=16;					// HERALDRY
							// ALSO REMEMBER, THAT ELEMENT IDS BEGIN FROM 0

int added_elements = 0;
int active_elements = 0;

int is_heraldry;

int store_id = 0;

std::string SAVED_FILENAME;

sf::RenderTexture texture;

sf::Sprite sprite_heraldry;
sf::Texture logo_texture;
sf::Sprite sprite_addiheraldry;

tgui::Label::Ptr label_rgb = tgui::Label::create();
tgui::Label::Ptr label_pos_xy = tgui::Label::create();
tgui::Label::Ptr label_scale_xy = tgui::Label::create();
tgui::Label::Ptr label_rotated = tgui::Label::create();

tgui::Slider::Ptr slider_r = tgui::Slider::create();
tgui::Slider::Ptr slider_g = tgui::Slider::create();
tgui::Slider::Ptr slider_b = tgui::Slider::create();
tgui::Label::Ptr selected_e_name = tgui::Label::create();

tgui::Slider::Ptr slider_pos_x = tgui::Slider::create();
tgui::Slider::Ptr slider_pos_y = tgui::Slider::create();

tgui::Slider::Ptr slider_scale_x = tgui::Slider::create();
tgui::Slider::Ptr slider_scale_y = tgui::Slider::create();

tgui::Slider::Ptr rotation = tgui::Slider::create();

tgui::ScrollablePanel::Ptr elements_panel = tgui::ScrollablePanel::create();
tgui::HorizontalWrap::Ptr elements_wrapper = tgui::HorizontalWrap::create();

int wrapper_y = 32;

tgui::BitmapButton::Ptr add_flag_button = tgui::BitmapButton::create();
tgui::Button::Ptr save_button = tgui::Button::create();
tgui::Button::Ptr save_button_tga = tgui::Button::create();
tgui::Button::Ptr save_button_bmp = tgui::Button::create();
tgui::Button::Ptr save_button_jpeg = tgui::Button::create();
tgui::BitmapButton::Ptr buttons_elements[32];


///////////////////////////ELEMENT DEFINES////////////////////////////
//	type: 1 - flag, 2 - heraldic element, 3 - additional heraldry	//
//	frame: [0 ... 10] - frame to load from assets				 	//
//	rgb: [255, 255, 255] - element colors						 	//
//	pos_x, pos_y: positions of element								//
//	scale_x, scale_y: element scaling								//
//	element_texture: texture to load								//
//	element_sprite: sprite of the element							//
//////////////////////////////////////////////////////////////////////

struct element{
	int type;
	int frame;
	int r, g, b;
	int pos_x, pos_y;
	float scale_x, scale_y;
	
	sf::Texture element_texture;
	sf::Sprite element_sprite;
};

element elements[32];

		//////////////////////////////////////////////
		// [i] - information for debugging			//
		// [^] - successfully completed operation	//
		//////////////////////////////////////////////

void heraldry_panel_heraldry(){
		is_heraldry = 0;
	}

void heraldry_panel_addiheraldry(){
		is_heraldry = 1;
	}

void ptr_buffer(int l){
		store_id = l;
		std::cout<<"[i] stored: "<<" "<< store_id <<std::endl;
		
		slider_r->setValue(elements[store_id].r);
		slider_g->setValue(elements[store_id].g);
		slider_b->setValue(elements[store_id].b);
		
		slider_scale_x->setValue(elements[store_id].scale_x);
		slider_scale_y->setValue(elements[store_id].scale_y);
		
		slider_pos_x->setValue(elements[store_id].pos_x);
		slider_pos_y->setValue(elements[store_id].pos_y);
		
		rotation->setValue(elements[store_id].element_sprite.getRotation());
		
		slider_r->connect("ValueChanged", [&](){ elements[store_id].r = slider_r->getValue(); });
		slider_g->connect("ValueChanged", [&](){ elements[store_id].g = slider_g->getValue(); });
		slider_b->connect("ValueChanged", [&](){ elements[store_id].b = slider_b->getValue(); });
		slider_pos_x->connect("ValueChanged", [&](){ elements[store_id].pos_x = slider_pos_x->getValue(); });
		slider_pos_y->connect("ValueChanged", [&](){ elements[store_id].pos_y = slider_pos_y->getValue(); });
		slider_scale_x->connect("ValueChanged", [&](){ elements[store_id].scale_x = slider_scale_x->getValue(); });
		slider_scale_y->connect("ValueChanged", [&](){ elements[store_id].scale_y = slider_scale_y->getValue(); });
		rotation->connect("ValueChanged", [&](){ elements[store_id].element_sprite.setRotation(rotation->getValue()); });
		
		selected_e_name->setText("Selected id: "+ std::to_string(store_id)+" Frame: "+ std::to_string(elements[store_id].frame));
	}

void element_alloc(){
		for(int i=0; i<32; i++){
				if(elements[i].type == 1 || elements[i].type == 2 || elements[i].type == 3)
				active_elements = i;
			}
	}

void create_element_button(){
		buttons_elements[active_elements+1] = tgui::BitmapButton::create();
		buttons_elements[active_elements+1]->setSize(125,32);
		if(elements[active_elements+1].type == 1){
			buttons_elements[active_elements+1]->setImage("assets/flagmain.png");
		}
		if(elements[active_elements+1].type == 2){
			buttons_elements[active_elements+1]->setImage("assets/heraldry/"+std::to_string(elements[active_elements+1].frame)+".png");
		}
		if(elements[active_elements+1].type == 3){
			buttons_elements[active_elements+1]->setImage("assets/additional heraldry/"+std::to_string(elements[active_elements+1].frame)+".png");
		}
		buttons_elements[active_elements+1]->setImageScaling(1);
		buttons_elements[active_elements+1]->connect("pressed", ptr_buffer, active_elements+1);
		wrapper_y += 32;
		std::cout<<wrapper_y<<std::endl;
		elements_wrapper->add(buttons_elements[active_elements+1]);
	
	}

void add_flagmain(tgui::Widget::Ptr widget, const std::string& signalName){
		added_elements++;
		element_alloc();
		
		elements[active_elements+1].type = 1;
		elements[active_elements+1].frame = 1;
		elements[active_elements+1].r = 255;
		elements[active_elements+1].g = 255;
		elements[active_elements+1].b = 0;
		
		create_element_button();
		std::cout<<"[i] added flag"<<" "<< added_elements <<std::endl;
		std::cout<< elements[active_elements+1].type <<" "<< elements[active_elements+1].frame<<std::endl;
		active_elements = 0;
	}

void add_element_heraldry(int i){
		added_elements++;
		element_alloc();
		
		elements[active_elements+1].type = 2;
		elements[active_elements+1].frame = i;
		elements[active_elements+1].r = 0;
		elements[active_elements+1].g = 255;
		elements[active_elements+1].b = 255;
		elements[active_elements+1].scale_x = 1;
		elements[active_elements+1].scale_y = 1;
		
		create_element_button();
		std::cout<<"[i] added element"<<" "<< added_elements <<std::endl;
		std::cout<< elements[active_elements+1].type <<" "<< elements[active_elements+1].frame<<std::endl;
		active_elements = 0;
	}

void add_element_addiheraldry(int j){
		added_elements++;
		element_alloc();
		
		elements[active_elements+1].type = 3;
		elements[active_elements+1].frame = j;
		elements[active_elements+1].r = 255;
		elements[active_elements+1].g = 0;
		elements[active_elements+1].b = 255;
		elements[active_elements+1].scale_x = 1;
		elements[active_elements+1].scale_y = 1;
		
		create_element_button();
		std::cout<<"[i] added element"<<" "<< added_elements <<std::endl;
		std::cout<< elements[active_elements+1].type <<" "<< elements[active_elements+1].frame<<std::endl;
		active_elements = 0;
	}
	

int main()
{
	using namespace std;
	sf::RenderWindow window(sf::VideoMode(w, h, 32), "Flagbd version 0.4");
	tgui::Gui gui{window}; 
	
	srand(time(0));
	
	if (!texture.create(300, 200))
	{
		return -1;
	}
	
	logo_texture.loadFromFile("assets/gui/flagbd_logo_80x16.png");
	sf::Sprite logo_sprite(logo_texture);
	
	logo_sprite.setPosition(520,2);
	
	///////////////////////// GUI/PANELS ///////////////////////////
	tgui::ScrollablePanel::Ptr properties_panel = tgui::ScrollablePanel::create();
	tgui::ScrollablePanel::Ptr heraldry_panel = tgui::ScrollablePanel::create();
	tgui::ScrollablePanel::Ptr addiheraldry_panel = tgui::ScrollablePanel::create();
	tgui::Panel::Ptr stripes_panel = tgui::Panel::create();
	
	//////////////////////// GUI/BUTTONS ///////////////////////////
	tgui::BitmapButton::Ptr button_heraldry = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr button_addiheraldry = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr del_flag_button = tgui::BitmapButton::create();
	
	tgui::BitmapButton::Ptr bicolor_group1 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr bicolor_group2 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr fourcolor_group1 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr fourcolor_group2 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr tricolor_group1 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr tricolor_group2 = tgui::BitmapButton::create();
	
	tgui::BitmapButton::Ptr buttons_heraldry[heraldry];
	tgui::BitmapButton::Ptr buttons_addiheraldry[adh];
	
	for(int i=0; i<heraldry; i++){
		buttons_heraldry[i] = tgui::BitmapButton::create();
		buttons_heraldry[i]->setSize(36,36);
		if(i<10){
			buttons_heraldry[i]->setPosition(1+(i*36),1);
		}
		if(i==10){
			buttons_heraldry[i]->setPosition(1,1+(i*3.7));
		}
		if(i>10){
			buttons_heraldry[i]->setPosition(1+((i-10)*36),38);
		}
		buttons_heraldry[i]->setImage("assets/heraldry/"+to_string(i)+".png");
		buttons_heraldry[i]->setImageScaling(1);
		heraldry_panel->add(buttons_heraldry[i]);
		buttons_heraldry[i]->connect("pressed", add_element_heraldry, i);
	
		//std::cout<<"Heraldry: "<< i <<std::endl; 					// for debugging
	}
	
	for(int j=0; j<adh; j++){
		buttons_addiheraldry[j] = tgui::BitmapButton::create();
		buttons_addiheraldry[j]->setSize(36,36);
		if(j<10){
			buttons_addiheraldry[j]->setPosition(1+(j*36),1);
		}
		if(j==10){
			buttons_addiheraldry[j]->setPosition(1,1+(j*3.7));
		}
		if(j>10){
			buttons_addiheraldry[j]->setPosition(1+((j-10)*36),38);
		}
		buttons_addiheraldry[j]->setImage("assets/additional heraldry/"+to_string(j)+".png");
		buttons_addiheraldry[j]->setImageScaling(1);
		addiheraldry_panel->add(buttons_addiheraldry[j]);
		buttons_addiheraldry[j]->connect("pressed", add_element_addiheraldry, j);
	
		//std::cout<<"Additional heraldry: "<< j <<std::endl; 					// for debugging
	}
	
	////////////////////// GUI DEFINES /////////////////////////////
	properties_panel->setSize(288,191);
	elements_panel->setSize(144,165);
	heraldry_panel->setSize(278,152);
	addiheraldry_panel->setSize(278,152);
	stripes_panel->setSize(143,165);
	properties_panel->setPosition(313,20);
	elements_panel->setPosition(5,232);
	heraldry_panel->setPosition(320,245);
	addiheraldry_panel->setPosition(320,245);
	stripes_panel->setPosition(151,232);
	
	
	
	button_heraldry->setImage("assets/gui/heraldry_icon.png");
	button_addiheraldry->setImage("assets/gui/additional_heraldry_icon.png");
	add_flag_button->setImage("assets/gui/flagmain_icon.png");
	del_flag_button->setImage("assets/gui/delete_element_icon.png");
	bicolor_group1->setImage("assets/gui/bicolor1_icon.png");
	bicolor_group2->setImage("assets/gui/bicolor2_icon.png");
	fourcolor_group1->setImage("assets/gui/fourcolor1_icon.png");
	fourcolor_group2->setImage("assets/gui/fourcolor2_icon.png");
	tricolor_group1->setImage("assets/gui/tricolor1_icon.png");
	tricolor_group2->setImage("assets/gui/tricolor2_icon.png");
	
	save_button->setText("Save(.png)");
	save_button_tga->setText("Save(.tga)");
	save_button_bmp->setText("Save(.bmp)");
	save_button_jpeg->setText("Save(.jpg)");
	save_button->setSize(75,20);
	save_button_tga->setSize(75,20);
	save_button_bmp->setSize(75,20);
	save_button_jpeg->setSize(75,20);
	button_heraldry->setSize(34,24);
	button_addiheraldry->setSize(34,24);
	add_flag_button->setSize(34,24);
	del_flag_button->setSize(34,24);
	button_heraldry->setPosition(328,220);
	button_addiheraldry->setPosition(365,220);
	add_flag_button->setPosition(402,220);
	del_flag_button->setPosition(439,220);
	save_button->setPosition(5,0);
	save_button_tga->setPosition(81,0);
	save_button_bmp->setPosition(156,0);
	save_button_jpeg->setPosition(231,0);
	
	bicolor_group1->setSize(36,36);
	bicolor_group2->setSize(36,36);
	fourcolor_group1->setSize(36,36);
	fourcolor_group2->setSize(36,36);
	tricolor_group1->setSize(36,36);
	tricolor_group2->setSize(36,36);
	
	bicolor_group1->setPosition(15,2);
	bicolor_group2->setPosition(52,2);
	fourcolor_group1->setPosition(89,2);
	fourcolor_group2->setPosition(15,38);
	tricolor_group1->setPosition(52,38);
	tricolor_group2->setPosition(89,38);
	
	button_heraldry->connect("pressed", heraldry_panel_heraldry);
	button_addiheraldry->connect("pressed", heraldry_panel_addiheraldry);
	add_flag_button->connect("pressed", add_flagmain);
	del_flag_button->connect("pressed", [&](){ elements[store_id] = elements[0]; elements_wrapper->remove(buttons_elements[store_id]); added_elements--; wrapper_y -= 32; std::cout<<added_elements<<" "<<wrapper_y<<std::endl; });
	save_button->connect("pressed", [&](){ texture.getTexture().copyToImage().saveToFile(SAVED_FILENAME+".png"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".png"<<std::endl; });
	save_button_bmp->connect("pressed", [&](){ texture.getTexture().copyToImage().saveToFile(SAVED_FILENAME+".bmp"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".bmp"<<std::endl; });
	save_button_tga->connect("pressed", [&](){ texture.getTexture().copyToImage().saveToFile(SAVED_FILENAME+".tga"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".tga"<<std::endl; });
	save_button_jpeg->connect("pressed", [&](){ texture.getTexture().copyToImage().saveToFile(SAVED_FILENAME+".jpg"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".jpg"<<std::endl; });
	
	bicolor_group1->connect("pressed", [&](){ if(elements[store_id].frame == 14 && store_id < 3 ) { elements[store_id].pos_x = 150; elements[store_id].pos_y = store_id * 50; if(store_id == 2) { elements[store_id].pos_y += 50; } } });
	bicolor_group2->connect("pressed", [&](){ if(elements[store_id].frame == 15 && store_id < 3 ) { elements[store_id].pos_x = store_id * 75; if(store_id == 2) { elements[store_id].pos_x += 75; } elements[store_id].pos_y = 100; } });
	fourcolor_group1->connect("pressed", [&](){ if(elements[store_id].frame == 17 && store_id < 5 ) { if(store_id == 1) { elements[store_id].pos_y = 25; } if(store_id == 2) { elements[store_id].pos_y = 75; } if(store_id == 3) { elements[store_id].pos_y = 125; } if(store_id == 4) { elements[store_id].pos_y = 175; } elements[store_id].pos_x = 150; } });
	fourcolor_group2->connect("pressed", [&](){ if(elements[store_id].frame == 13 && store_id < 5 ) { if(store_id == 1) { elements[store_id].pos_x = 38; } if(store_id == 2) { elements[store_id].pos_x = 113; } if(store_id == 3) { elements[store_id].pos_x = 188; } if(store_id == 4) { elements[store_id].pos_x = 263; } elements[store_id].pos_y = 100; } });
	tricolor_group1->connect("pressed", [&](){ if(elements[store_id].frame == 16 && store_id < 4 ) { if(store_id == 1) { elements[store_id].pos_y = store_id * 33; } if(store_id == 2) { elements[store_id].pos_y = (store_id + 1) * 33; } if(store_id == 3) { elements[store_id].pos_y = 166; } elements[store_id].pos_x = 150; } });
	tricolor_group2->connect("pressed", [&](){ if(elements[store_id].frame == 12 && store_id < 4 ) { if(store_id == 1) { elements[store_id].pos_x = store_id * 50; } if(store_id == 2){ elements[store_id].pos_x = (store_id + 1) * 50; } if(store_id == 3){ elements[store_id].pos_x = (store_id + 2) * 50; }  elements[store_id].pos_y = 100; } });
	
	elements_panel->add(elements_wrapper);
	
	gui.add(properties_panel);
	gui.add(elements_panel);
	gui.add(heraldry_panel);
	gui.remove(addiheraldry_panel);
	gui.add(stripes_panel);
	
	gui.add(button_heraldry);
	gui.add(button_addiheraldry);
	gui.add(add_flag_button);
	gui.add(del_flag_button);
	gui.add(save_button);
	gui.add(save_button_bmp);
	gui.add(save_button_jpeg);
	gui.add(save_button_tga);
	
	/////////////////////// PROPERTIES DEFINES /////////////////////////
	tgui::Label::Ptr label_r = tgui::Label::create();
	tgui::Label::Ptr label_g = tgui::Label::create();
	tgui::Label::Ptr label_b = tgui::Label::create();
	tgui::Label::Ptr label_pos_x = tgui::Label::create();
	tgui::Label::Ptr label_pos_y = tgui::Label::create();
	tgui::Label::Ptr label_scale_x = tgui::Label::create();
	tgui::Label::Ptr label_scale_y = tgui::Label::create();
	tgui::Label::Ptr label_rotate = tgui::Label::create();
	
	slider_r->setMinimum(0);
	slider_r->setMaximum(255);
	slider_g->setMinimum(0);
	slider_g->setMaximum(255);
	slider_b->setMinimum(0);
	slider_b->setMaximum(255);
	
	slider_pos_x->setMinimum(0);
	slider_pos_x->setMaximum(300);
	slider_pos_y->setMinimum(0);
	slider_pos_y->setMaximum(200);
	
	slider_scale_x->setMinimum(-1);
	slider_scale_x->setMaximum(5);
	slider_scale_y->setMinimum(-1);
	slider_scale_y->setMaximum(5);
	slider_scale_x->setStep(0.1);
	slider_scale_y->setStep(0.1);
	
	rotation->setMinimum(0);
	rotation->setMaximum(360);
	
	slider_r->setSize(243,11);
	slider_g->setSize(243,11);
	slider_b->setSize(243,11);
	slider_r->setPosition(26,28);
	slider_g->setPosition(26,48);
	slider_b->setPosition(26,68);
	
	slider_pos_x->setSize(211,10);
	slider_pos_y->setSize(211,10);
	slider_pos_x->setPosition(56,100);
	slider_pos_y->setPosition(56,120);
	
	selected_e_name->setSize(264,16);
	selected_e_name->setPosition(4,4);
	
	slider_scale_x->setSize(201,10);
	slider_scale_y->setSize(201,10);
	slider_scale_x->setPosition(66,144);
	slider_scale_y->setPosition(66,164);
	
	rotation->setSize(201,10);
	rotation->setPosition(60,204);
	
	label_r->setSize(20,16);
	label_g->setSize(20,16);
	label_b->setSize(20,16);
	label_r->setPosition(6,28);
	label_g->setPosition(6,48);
	label_b->setPosition(6,68);
	
	label_scale_x->setSize(55,16);
	label_scale_y->setSize(55,16);
	label_scale_x->setPosition(5,144);
	label_scale_y->setPosition(5,164);
	
	label_rgb->setSize(90,15);
	label_rgb->setTextSize(9);
	label_rgb->setPosition(200,80);
	label_pos_xy->setSize(90,15);
	label_pos_xy->setTextSize(9);
	label_pos_xy->setPosition(218,130);
	label_scale_xy->setSize(150,15);
	label_scale_xy->setTextSize(9);
	label_scale_xy->setPosition(150,180);
	label_rotated->setSize(100,15);
	label_rotated->setTextSize(9);
	label_rotated->setPosition(200,220);
	
	label_rotate->setSize(55,16);
	label_rotate->setPosition(5,200);
	
	label_r->setText("R:");
	label_g->setText("G:");
	label_b->setText("B:");
	
	label_scale_x->setText("scale x:");
	label_scale_y->setText("scale y:");
	
	label_rotate->setText("rotate:");
	
	label_pos_x->setSize(51,15);
	label_pos_y->setSize(51,15);
	label_pos_x->setPosition(6,100);
	label_pos_y->setPosition(6,120);
	
	label_pos_x->setText("pos_x:");
	label_pos_y->setText("pos_y:");
	
	properties_panel->add(label_r);
	properties_panel->add(label_g);
	properties_panel->add(label_b);
	
	properties_panel->add(label_pos_x);
	properties_panel->add(label_pos_y);
	
	properties_panel->add(slider_r);
	properties_panel->add(slider_g);
	properties_panel->add(slider_b);
	
	properties_panel->add(slider_pos_x);
	properties_panel->add(slider_pos_y);
	
	properties_panel->add(selected_e_name);
	properties_panel->add(label_rgb);
	properties_panel->add(label_pos_xy);
	properties_panel->add(label_scale_xy);
	
	properties_panel->add(slider_scale_x);
	properties_panel->add(slider_scale_y);
	properties_panel->add(label_scale_x);
	properties_panel->add(label_scale_y);
	properties_panel->add(rotation);
	properties_panel->add(label_rotate);
	properties_panel->add(label_rotated);
	
	stripes_panel->add(bicolor_group1);
	stripes_panel->add(bicolor_group2);
	stripes_panel->add(fourcolor_group1);
	stripes_panel->add(fourcolor_group2);
	stripes_panel->add(tricolor_group1);
	stripes_panel->add(tricolor_group2);
	
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			gui.handleEvent(event); // Pass the event to the widgets
		}
		
		elements_wrapper->setSize(128,wrapper_y);
		
		texture.clear(sf::Color::White);
		
		/////////////////////// FLAG ELEMENTS //////////////////////////
		texture.display();
		
		if(is_heraldry == 0){
			gui.add(heraldry_panel);
			gui.remove(addiheraldry_panel);
		}
		
		if(is_heraldry == 1){
			gui.remove(heraldry_panel);
			gui.add(addiheraldry_panel);
		}
		
		////////////////// FLAG ELEMENTS PROCESS ///////////////////////
	
		for(int i=0; i<32; i++){
			if(elements[i].type == 1){
					elements[i].element_texture.loadFromFile("assets/flagmain.png");
					elements[i].element_sprite.setTexture(elements[i].element_texture);
					elements[i].element_sprite.setColor(sf::Color(elements[i].r, elements[i].g, elements[i].b));
				}
			if(elements[i].type == 2){
					elements[i].element_texture.loadFromFile("assets/heraldry/"+to_string(elements[i].frame)+".png");
					elements[i].element_sprite.setTexture(elements[i].element_texture);
					elements[i].element_sprite.setColor(sf::Color(elements[i].r, elements[i].g, elements[i].b));
					elements[i].element_sprite.setScale(elements[i].scale_x, elements[i].scale_y);
					elements[i].element_sprite.setOrigin(elements[i].element_sprite.getLocalBounds().width/2, elements[i].element_sprite.getLocalBounds().height/2);
					elements[i].element_sprite.setPosition(elements[i].pos_x,elements[i].pos_y);
				}
			if(elements[i].type == 3){
					elements[i].element_texture.loadFromFile("assets/additional heraldry/"+to_string(elements[i].frame)+".png");
					elements[i].element_sprite.setTexture(elements[i].element_texture);
					elements[i].element_sprite.setColor(sf::Color(elements[i].r, elements[i].g, elements[i].b));
					elements[i].element_sprite.setScale(elements[i].scale_x, elements[i].scale_y);
					elements[i].element_sprite.setOrigin(elements[i].element_sprite.getLocalBounds().width/2, elements[i].element_sprite.getLocalBounds().height/2);
					elements[i].element_sprite.setPosition(elements[i].pos_x,elements[i].pos_y);
				}
		}
		label_rgb->setText("("+std::to_string(elements[store_id].r)+"; "+std::to_string(elements[store_id].g)+"; "+std::to_string(elements[store_id].b)+")");
		label_pos_xy->setText("("+std::to_string(elements[store_id].pos_x)+"; "+std::to_string(elements[store_id].pos_y)+")");
		label_scale_xy->setText("("+std::to_string(elements[store_id].scale_x)+"; "+std::to_string(elements[store_id].scale_y)+")");
		label_rotated->setText("("+std::to_string(elements[store_id].element_sprite.getRotation())+")");
		
		if(store_id == 0){
				del_flag_button->setEnabled(false);
			}
		if(store_id > 0){
				del_flag_button->setEnabled(true);
			}
			
		if(elements[store_id].frame != 14){
				bicolor_group1->setEnabled(false);
			}
		if(elements[store_id].frame == 14){
				bicolor_group1->setEnabled(true);
			}
		
		if(elements[store_id].frame != 15){
				bicolor_group2->setEnabled(false);
			}
		if(elements[store_id].frame == 15){
				bicolor_group2->setEnabled(true);
			}
			
		if(elements[store_id].frame != 17){
				fourcolor_group1->setEnabled(false);
			}
		if(elements[store_id].frame == 17){
				fourcolor_group1->setEnabled(true);
			}
		
		if(elements[store_id].frame != 13){
				fourcolor_group2->setEnabled(false);
			}
		if(elements[store_id].frame == 13){
				fourcolor_group2->setEnabled(true);
			}
		
		if(elements[store_id].frame != 16){
				tricolor_group1->setEnabled(false);
			}
		if(elements[store_id].frame == 16){
				tricolor_group1->setEnabled(true);
			}
		
		if(elements[store_id].frame != 12){
				tricolor_group2->setEnabled(false);
			}
		if(elements[store_id].frame == 12){
				tricolor_group2->setEnabled(true);
			}
		
		for(int i=0; i<32; i++){
			texture.draw(elements[i].element_sprite);
		}
		
		
		sf::Sprite rendered_texture(texture.getTexture());
		rendered_texture.setPosition(5,20);
		
		int svr = (added_elements * 1024)/16;
		
		int pmvar = rand() % 4096;
		
		SAVED_FILENAME = std::to_string(added_elements+svr+elements[added_elements].r+elements[added_elements].g+elements[added_elements].b+pmvar);
		
		window.clear();
		window.draw(rendered_texture);
		window.draw(logo_sprite);
		gui.draw(); // Draw all widgets
		window.display();
	}
}
