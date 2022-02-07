#include <TGUI/TGUI.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>


int w=600;
int h=400;

//////////////////////////////////////////////////////////////////
// 				ADVICE FOR ADDING NEW ELEMENTS:					//
//	It's recommended(but not necessary) for heraldry textures	//
//	to be white, as it affects how the color of the element 	//
//	will change.												//
//////////////////////////////////////////////////////////////////

int heraldry=20;			// CHANGE THESE VARIABLES AFTER ADDIG NEW
int adh=16;					// HERALDRY
							// ALSO REMEMBER, THAT ELEMENT IDS BEGIN FROM 0
							// adh = assets/additional heraldry
							// heraldry = assets/heraldry

int added_elements = 0;
int active_elements = 0;

int is_heraldry;

int store_id = 0;

// Saving variables for copy-pasting //
int saved_r = 0;
int saved_g = 0;
int saved_b = 0;

float saved_scale_x = 1;
float saved_scale_y = 1;

float saved_rotation = 0;

int saved_pos_x = 0;
int saved_pos_y = 0;
//////////////////////////////////////

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

tgui::CheckBox::Ptr smooth_cb = tgui::CheckBox::create();

tgui::TextBox::Ptr filename = tgui::TextBox::create();

tgui::EditBox::Ptr x_size = tgui::EditBox::create();
tgui::EditBox::Ptr y_size = tgui::EditBox::create();

tgui::ScrollablePanel::Ptr elements_panel = tgui::ScrollablePanel::create();
tgui::HorizontalWrap::Ptr elements_wrapper = tgui::HorizontalWrap::create();

tgui::HorizontalWrap::Ptr heraldry_wrapper = tgui::HorizontalWrap::create();
tgui::HorizontalWrap::Ptr addiheraldry_wrapper = tgui::HorizontalWrap::create();

tgui::MenuBar::Ptr menu_save = tgui::MenuBar::create();

int wrapper_y = 32;

tgui::BitmapButton::Ptr add_flag_button = tgui::BitmapButton::create();
tgui::BitmapButton::Ptr buttons_elements[32];


///////////////////////////ELEMENT DEFINES////////////////////////////
//	type: 1 - flag, 2 - heraldic element, 3 - additional heraldry	//
//	frame: [0 ... n] - frame to load from assets				 	//
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

void element_smooth_on(){
		for(int i=0; i<32; i++)
			elements[i].element_texture.setSmooth(true);
	}

void element_smooth_off(){
		for(int i=0; i<32; i++)
			elements[i].element_texture.setSmooth(false);
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
		elements[active_elements+1].pos_x = 150;
		elements[active_elements+1].pos_y = 100;
		
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
		elements[active_elements+1].pos_x = 150;
		elements[active_elements+1].pos_y = 100;
		
		
		create_element_button();
		std::cout<<"[i] added element"<<" "<< added_elements <<std::endl;
		std::cout<< elements[active_elements+1].type <<" "<< elements[active_elements+1].frame<<std::endl;
		active_elements = 0;
	}
	

int main()
{
	using namespace std;
	sf::RenderWindow window(sf::VideoMode(w, h, 32), "Flagbd version 0.6");
	tgui::Gui gui{window}; 
	
	window.setFramerateLimit(60);
	//////////////////////////////////////////////
	//	Solves issues with high CPU usage,		//
	//	if usage is still high, try lowering or	//
	//	disabling the FPS limit.				//
	//	Also try checking your GPU drivers.		//
	//////////////////////////////////////////////
	
	if (!texture.create(300, 200))
	{
		return EXIT_FAILURE;
	}
	
	logo_texture.loadFromFile("assets/gui/flagbd_logo_80x16.png");
	sf::Sprite logo_sprite(logo_texture);
	
	logo_sprite.setPosition(520,2);
	
	///////////////////////// GUI/PANELS ///////////////////////////
	tgui::ScrollablePanel::Ptr properties_panel = tgui::ScrollablePanel::create();
	tgui::ScrollablePanel::Ptr heraldry_panel = tgui::ScrollablePanel::create();
	tgui::ScrollablePanel::Ptr addiheraldry_panel = tgui::ScrollablePanel::create();
	tgui::Panel::Ptr edit_panel = tgui::Panel::create();
	
	//////////////////////// GUI/BUTTONS ///////////////////////////
	tgui::BitmapButton::Ptr button_heraldry = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr button_addiheraldry = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr del_flag_button = tgui::BitmapButton::create();
	
	tgui::BitmapButton::Ptr rotatemin45 = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_color = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_color = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_scale = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_scale = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_rotation = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_rotation = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_position = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_position = tgui::BitmapButton::create();
	
	tgui::BitmapButton::Ptr buttons_heraldry[heraldry];
	tgui::BitmapButton::Ptr buttons_addiheraldry[adh];
	
	
	/////////////////////////// WRAPPERS ///////////////////////////////
	heraldry_wrapper->setSize(264, (36*heraldry)/5);
	heraldry_wrapper->setPosition(5,5);
	heraldry_panel->add(heraldry_wrapper);
	
	addiheraldry_wrapper->setPosition(5,5);
	addiheraldry_wrapper->setSize(264, (36*adh)/5);
	addiheraldry_panel->add(addiheraldry_wrapper);
	
	for(int i=0; i<heraldry; i++){
		buttons_heraldry[i] = tgui::BitmapButton::create();
		buttons_heraldry[i]->setSize(36,36);
		buttons_heraldry[i]->setImage("assets/heraldry/"+to_string(i)+".png");
		buttons_heraldry[i]->setImageScaling(1);
		heraldry_wrapper->add(buttons_heraldry[i]);
		buttons_heraldry[i]->connect("pressed", add_element_heraldry, i);
	
		//std::cout<<"Heraldry: "<< i <<std::endl; 					// for debugging
	}
	
	for(int j=0; j<adh; j++){
		buttons_addiheraldry[j] = tgui::BitmapButton::create();
		buttons_addiheraldry[j]->setSize(36,36);
		buttons_addiheraldry[j]->setImage("assets/additional heraldry/"+to_string(j)+".png");
		buttons_addiheraldry[j]->setImageScaling(1);
		addiheraldry_wrapper->add(buttons_addiheraldry[j]);
		buttons_addiheraldry[j]->connect("pressed", add_element_addiheraldry, j);
	
		//std::cout<<"Additional heraldry: "<< j <<std::endl; 					// for debugging
	}
	
	////////////////////// GUI DEFINES /////////////////////////////
	properties_panel->setSize(288,191);
	elements_panel->setSize(144,165);
	heraldry_panel->setSize(278,152);
	addiheraldry_panel->setSize(278,152);
	edit_panel->setSize(143,165);
	properties_panel->setPosition(313,20);
	elements_panel->setPosition(5,232);
	heraldry_panel->setPosition(320,245);
	addiheraldry_panel->setPosition(320,245);
	edit_panel->setPosition(151,232);
	
	
	
	button_heraldry->setImage("assets/gui/heraldry_icon.png");
	button_addiheraldry->setImage("assets/gui/additional_heraldry_icon.png");
	add_flag_button->setImage("assets/gui/flagmain_icon.png");
	del_flag_button->setImage("assets/gui/delete_element_icon.png");
	rotatemin45->setImage("assets/gui/icon_-45d.png");
	copy_color->setImage("assets/gui/copy_color_icon.png");
	paste_color->setImage("assets/gui/paste_color_icon.png");
	copy_scale->setImage("assets/gui/copy_scale_icon.png");
	paste_scale->setImage("assets/gui/paste_scale_icon.png");
	copy_rotation->setImage("assets/gui/copy_rotation_icon.png");
	paste_rotation->setImage("assets/gui/paste_rotation_icon.png");
	copy_position->setImage("assets/gui/copy_position_icon.png");
	paste_position->setImage("assets/gui/paste_position_icon.png");
	
	
	button_heraldry->setSize(34,24);
	button_addiheraldry->setSize(34,24);
	add_flag_button->setSize(34,24);
	del_flag_button->setSize(34,24);
	button_heraldry->setPosition(328,220);
	button_addiheraldry->setPosition(365,220);
	add_flag_button->setPosition(402,220);
	del_flag_button->setPosition(439,220);
	
	menu_save->setSize(59,18);
	menu_save->addMenu("Save...");
	menu_save->addMenuItem("Save...", "Save .jpg");
	menu_save->addMenuItem("Save...", "Save .png");
	menu_save->addMenuItem("Save...", "Save .tga");
	menu_save->addMenuItem("Save...", "Save .bmp");
	
	menu_save->connectMenuItem({"Save...", "Save .jpg"}, [&](){ std::string s_x = x_size->getText(); std::string s_y = y_size->getText(); int x,y; x = std::stoi(s_x); y = std::stoi(s_y); sf::Sprite resized_flag(texture.getTexture()); sf::RenderTexture resized_image; resized_image.create(x, y); sf::Vector2u size_texture = texture.getSize(); float scale_x = (float) x / size_texture.x; float scale_y = (float) y / size_texture.y; resized_flag.setScale(scale_x,scale_y); resized_image.draw(resized_flag); resized_image.display(); resized_image.getTexture().copyToImage().saveToFile(filename->getText()+".jpg"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".jpg"<<std::endl; });
	menu_save->connectMenuItem({"Save...", "Save .png"}, [&](){ std::string s_x = x_size->getText(); std::string s_y = y_size->getText(); int x,y; x = std::stoi(s_x); y = std::stoi(s_y); sf::Sprite resized_flag(texture.getTexture()); sf::RenderTexture resized_image; resized_image.create(x, y); sf::Vector2u size_texture = texture.getSize(); float scale_x = (float) x / size_texture.x; float scale_y = (float) y / size_texture.y; resized_flag.setScale(scale_x,scale_y); resized_image.draw(resized_flag); resized_image.display(); resized_image.getTexture().copyToImage().saveToFile(filename->getText()+".png"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".png"<<std::endl; });
	menu_save->connectMenuItem({"Save...", "Save .tga"}, [&](){ std::string s_x = x_size->getText(); std::string s_y = y_size->getText(); int x,y; x = std::stoi(s_x); y = std::stoi(s_y); sf::Sprite resized_flag(texture.getTexture()); sf::RenderTexture resized_image; resized_image.create(x, y); sf::Vector2u size_texture = texture.getSize(); float scale_x = (float) x / size_texture.x; float scale_y = (float) y / size_texture.y; resized_flag.setScale(scale_x,scale_y); resized_image.draw(resized_flag); resized_image.display(); resized_image.getTexture().copyToImage().saveToFile(filename->getText()+".tga"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".tga"<<std::endl; });
	menu_save->connectMenuItem({"Save...", "Save .bmp"}, [&](){ std::string s_x = x_size->getText(); std::string s_y = y_size->getText(); int x,y; x = std::stoi(s_x); y = std::stoi(s_y); sf::Sprite resized_flag(texture.getTexture()); sf::RenderTexture resized_image; resized_image.create(x, y); sf::Vector2u size_texture = texture.getSize(); float scale_x = (float) x / size_texture.x; float scale_y = (float) y / size_texture.y; resized_flag.setScale(scale_x,scale_y); resized_image.draw(resized_flag); resized_image.display(); resized_image.getTexture().copyToImage().saveToFile(filename->getText()+".bmp"); std::cout<<"[^] Saved file: "<<SAVED_FILENAME<<".bmp"<<std::endl; });
	
	rotatemin45->setSize(36,36);
	copy_color->setSize(36,36);
	paste_color->setSize(36,36);
	copy_scale->setSize(36,36);
	paste_scale->setSize(36,36);
	copy_rotation->setSize(36,36);
	paste_rotation->setSize(36,36);
	copy_position->setSize(36,36);
	paste_position->setSize(36,36);
	
	rotatemin45->setPosition(15,2);
	copy_color->setPosition(52,2);
	paste_color->setPosition(89,2);
	copy_scale->setPosition(15,38);
	paste_scale->setPosition(52,38);
	copy_rotation->setPosition(89,38);
	paste_rotation->setPosition(15,74);
	copy_position->setPosition(52,74);
	paste_position->setPosition(89,74);
	
	filename->setSize(200,20);
	filename->setPosition(60,0);
	filename->setText("Flagname");
	
	x_size->setSize(50,20);
	y_size->setSize(50,20);
	x_size->setPosition(261,0);
	y_size->setPosition(311,0);
	
	x_size->setInputValidator("[0-9]*");
	y_size->setInputValidator("[0-9]*");
	
	x_size->setMaximumCharacters(4);
	y_size->setMaximumCharacters(4);
	
	x_size->setText("300");
	y_size->setText("200");
	
	smooth_cb->setPosition(400,0);
	smooth_cb->setText("Smooth");
	smooth_cb->setSize(20,20);
	
	button_heraldry->connect("pressed", heraldry_panel_heraldry);
	button_addiheraldry->connect("pressed", heraldry_panel_addiheraldry);
	add_flag_button->connect("pressed", add_flagmain);
	del_flag_button->connect("pressed", [&](){ elements[store_id] = elements[0]; elements_wrapper->remove(buttons_elements[store_id]); added_elements--; wrapper_y -= 32; std::cout<<added_elements<<" "<<wrapper_y<<std::endl; });
	
	rotatemin45->connect("pressed", [&](){ elements[store_id].element_sprite.setRotation(315); });
	copy_color->connect("pressed", [&](){ saved_r = elements[store_id].r; saved_g = elements[store_id].g; saved_b = elements[store_id].b; });
	paste_color->connect("pressed", [&](){ elements[store_id].r = saved_r; elements[store_id].g = saved_g; elements[store_id].b = saved_b; });
	copy_scale->connect("pressed", [&](){ saved_scale_x = elements[store_id].scale_x; saved_scale_y = elements[store_id].scale_y; });
	paste_scale->connect("pressed", [&](){ elements[store_id].scale_x = saved_scale_x; elements[store_id].scale_y = saved_scale_y; });
	copy_rotation->connect("pressed", [&](){ saved_rotation = elements[store_id].element_sprite.getRotation(); });
	paste_rotation->connect("pressed", [&](){ elements[store_id].element_sprite.setRotation(saved_rotation); });
	copy_position->connect("pressed", [&](){ saved_pos_x = elements[store_id].pos_x; saved_pos_y = elements[store_id].pos_y; });
	paste_position->connect("pressed", [&](){ elements[store_id].pos_x = saved_pos_x; elements[store_id].pos_y = saved_pos_y; });
	
	elements_panel->add(elements_wrapper);
	
	gui.add(properties_panel);
	gui.add(elements_panel);
	gui.add(heraldry_panel);
	gui.remove(addiheraldry_panel);
	gui.add(edit_panel);
	
	gui.add(button_heraldry);
	gui.add(button_addiheraldry);
	gui.add(add_flag_button);
	gui.add(del_flag_button);
	gui.add(filename);
	gui.add(menu_save);
	gui.add(smooth_cb);
	gui.add(x_size);
	gui.add(y_size);
	
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
	
	edit_panel->add(rotatemin45);
	edit_panel->add(copy_color);
	edit_panel->add(paste_color);
	edit_panel->add(copy_scale);
	edit_panel->add(paste_scale);
	edit_panel->add(copy_rotation);
	edit_panel->add(paste_rotation);
	edit_panel->add(copy_position);
	edit_panel->add(paste_position);
	
	
	
	smooth_cb->connect("Checked", element_smooth_on);
	smooth_cb->connect("Unchecked", element_smooth_off);
	
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
		
		for(int i=0; i<32; i++){
			texture.draw(elements[i].element_sprite);
		}
		
		
		sf::Sprite rendered_texture(texture.getTexture());
		rendered_texture.setPosition(5,20);
		
		SAVED_FILENAME = filename->getText();
		
		window.clear();
		window.draw(rendered_texture);
		window.draw(logo_sprite);
		gui.draw(); // Draw all widgets
		window.display();
	}
}
