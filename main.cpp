#include <TGUI/TGUI.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <string>

int x,y;
int f_x, f_y;

////////////////////////// FLAGBD VARIABLES //////////////////////////
//	values are loaded from flagbd.cfg
//			MEANING OF VARIABLES:
//	FBD_VERSION = version of FlagBuilder
//	NUM_OF_ELEMENTS = number of elements to load from assets/elements,
//	write a path to new elements in the config when you increase the value
//	MAX_ELEMENTS = number of elements that can be drawn in the same time,
//	Flagbd will most probably crash when trying to go over this value
//////////////////////////////////////////////////////////////////////
std::string FBD_VERSION;
int NUM_OF_ELEMENTS;
int MAX_ELEMENTS;
//////////////////////////////////////////////////////////////////////

bool FILE_OPEN = false;
bool smooth_enabled = false;
std::string save_file_path;

std::string file_extension;
int added_elements = 0;
int active_elements = 0;
int remembered_elements = 0;
int store_id = 0;
int wrapper_y = 36;
float zoom_value=1;

// Saving variables for copy-pasting //
int saved_r = 0;
int saved_g = 0;
int saved_b = 0;

float saved_scale_x = 1;
float saved_scale_y = 1;

float saved_rotation = 0;

int saved_pos_x = 0;
int saved_pos_y = 0;
///////////////////////////////////////

///////////////////////////ADDING NEW ELEMENTS////////////////////////
//	FlagBuilder 0.7+ has flagbd.cfg file, in which one can define	//
//	how much heraldry Flagbd can load and write paths to textures of// 
//	the elements.													//
//	To increase the number of heraldry change the line 2 in config	//
//	Then add a path to the element after line 3 in flagbd.cfg		//
//////////////////////////////////////////////////////////////////////

///////////////////////////ELEMENT DEFINES////////////////////////////
//	path: path to the texture of the element						//
//	type: element type (heraldry or text)							//
//	rgb: [255, 255, 255] - element colors						 	//
//	pos_x, pos_y: positions of element								//
//	scale_x, scale_y: element scaling								//
//	e_rotation: element rotation(for saving)						//
//	element_texture: texture to load								//
//	element_sprite: sprite of the element							//
//	element_font: font to load from path(TEXT ELEMENTS ONLY)		//
//	element_text: element text(TEXT ELEMENTS ONLY)					//
//	element_str_text: element string(TEXT ELEMENTS, FOR SAVING ONLY)//
//	element_text_size: element character size						//
//	(TEXT ELEMENTS, FOR SAVING ONLY)								//
//////////////////////////////////////////////////////////////////////

struct element{
	std::string path;
	std::string type;
	int r, g, b;
	int pos_x, pos_y;
	float scale_x, scale_y;
	float e_rotation;
	
	sf::Texture element_texture;
	sf::Sprite element_sprite;
	sf::Font element_font;
	sf::Text element_text;
	std::string element_str_text;
	int element_text_size;
};
std::vector <element> elements;

struct texture_paths{
	std::string path;
};
std::vector <texture_paths> saved_tx_paths;

		//////////////////////////////////////////////////////
		// [i] - information for debugging					//
		// [^] - successfully completed operation			//
		// [x] - unsuccessfully completed operation(error)	//
		//////////////////////////////////////////////////////

sf::View display_view;
sf::RenderTexture texture;
sf::Sprite rendered_texture;
tgui::GuiSFML gui;

tgui::Label::Ptr label_rgb = tgui::Label::create();
tgui::Label::Ptr label_rotated = tgui::Label::create();
tgui::Slider::Ptr slider_r = tgui::Slider::create();
tgui::Slider::Ptr slider_g = tgui::Slider::create();
tgui::Slider::Ptr slider_b = tgui::Slider::create();
tgui::Label::Ptr selected_e_name = tgui::Label::create();
tgui::EditBox::Ptr ebox_pos_x = tgui::EditBox::create();
tgui::EditBox::Ptr ebox_pos_y = tgui::EditBox::create();

tgui::EditBox::Ptr ebox_scale_x = tgui::EditBox::create();
tgui::EditBox::Ptr ebox_scale_y = tgui::EditBox::create();

tgui::Slider::Ptr rotation = tgui::Slider::create();
auto ebox_font_path = tgui::EditBox::create();
auto ebox_str_text = tgui::EditBox::create();
auto ebox_str_size = tgui::EditBox::create();

auto rdbutton_font_r = tgui::RadioButton::create();
auto rdbutton_font_b = tgui::RadioButton::create();
auto rdbutton_font_i = tgui::RadioButton::create();
auto rdbutton_font_u = tgui::RadioButton::create();
auto rdbutton_font_st = tgui::RadioButton::create();

auto wrapper_layers = tgui::HorizontalWrap::create();
std::vector <tgui::BitmapButton::Ptr> buttons_elements;

void ptr_buffer(int l){
		store_id = l;
		std::cout<<"[i] stored: "<<" "<< store_id <<std::endl;
		
		if(elements[store_id].type == "heraldry"){
			ebox_font_path->setReadOnly(true);
			ebox_str_text->setReadOnly(true);
			ebox_str_size->setReadOnly(true);
			rdbutton_font_r->setEnabled(false);
			rdbutton_font_b->setEnabled(false);
			rdbutton_font_i->setEnabled(false);
			rdbutton_font_u->setEnabled(false);
			rdbutton_font_st->setEnabled(false);
		}
		else{
			ebox_font_path->setReadOnly(false);
			ebox_str_text->setReadOnly(false);
			ebox_str_size->setReadOnly(false);
			rdbutton_font_r->setEnabled(true);
			rdbutton_font_b->setEnabled(true);
			rdbutton_font_i->setEnabled(true);
			rdbutton_font_u->setEnabled(true);
			rdbutton_font_st->setEnabled(true);
			ebox_str_text->setText(elements[store_id].element_str_text);
			ebox_str_size->setText(std::to_string(elements[store_id].element_text_size));
			ebox_font_path->setText(elements[store_id].path);
		}
		
		slider_r->setValue(elements[store_id].r);
		slider_g->setValue(elements[store_id].g);
		slider_b->setValue(elements[store_id].b);
		
		ebox_scale_x->setText(std::to_string(elements[store_id].scale_x));
		ebox_scale_y->setText(std::to_string(elements[store_id].scale_y));
		
		ebox_pos_x->setText(std::to_string(elements[store_id].pos_x));
		ebox_pos_y->setText(std::to_string(elements[store_id].pos_y));
		
		if(elements[store_id].type == "heraldry"){
			rotation->setValue(elements[store_id].element_sprite.getRotation());
		}
		else{
			rotation->setValue(elements[store_id].element_text.getRotation());
		}
		
		slider_r->onValueChange([&](){ elements[store_id].r = slider_r->getValue(); });
		slider_g->onValueChange([&](){ elements[store_id].g = slider_g->getValue(); });
		slider_b->onValueChange([&](){ elements[store_id].b = slider_b->getValue(); });
		ebox_pos_x->onTextChange([&](){ tgui::String str_pos_x = ebox_pos_x->getText();; elements[store_id].pos_x = str_pos_x.toInt(); });
		ebox_pos_y->onTextChange([&](){ tgui::String str_pos_y = ebox_pos_y->getText();; elements[store_id].pos_y = str_pos_y.toInt(); });
		ebox_scale_x->onTextChange([&](){ tgui::String str_scale_x = ebox_scale_x->getText(); elements[store_id].scale_x = str_scale_x.toFloat(); });
		ebox_scale_y->onTextChange([&](){ tgui::String str_scale_y = ebox_scale_y->getText(); elements[store_id].scale_y = str_scale_y.toFloat(); });
		rotation->onValueChange([&](){ if(elements[store_id].type == "heraldry") { elements[store_id].element_sprite.setRotation(rotation->getValue()); } else { elements[store_id].element_text.setRotation(rotation->getValue()); } elements[store_id].e_rotation = rotation->getValue(); });
		ebox_font_path->onRightMousePress([&](){ if(elements[store_id].type == "text") { tgui::String f_path = ebox_font_path->getText(); elements[store_id].path = f_path.toStdString(); if(!elements[store_id].element_font.loadFromFile(f_path.toStdString().c_str())){ std::cout<< "[x] Failed to load font."<<std::endl; } elements[store_id].element_text.setFont(elements[store_id].element_font); } });
		ebox_str_text->onTextChange([&](){ tgui::String text = ebox_str_text->getText(); elements[store_id].element_str_text = text.toStdString(); elements[store_id].element_text.setString(text.toWideString()); });
		ebox_str_size->onTextChange([&](){ tgui::String size_str = ebox_str_size->getText(); elements[store_id].element_text_size = size_str.toInt(); elements[store_id].element_text.setCharacterSize(size_str.toInt()); });
		
		selected_e_name->setText("Selected id: "+ std::to_string(store_id));
	}

void find_previous_element(){
	for(int i=0; i<store_id; i++){
		if(!elements[i].type.empty())
			remembered_elements = i;
	}
	std::cout<<"[i] remembered id: "<<remembered_elements<<std::endl;
}

void find_next_element(){
	int stop = 0;
	for(int i=0; i<MAX_ELEMENTS; i++){
		if(!elements[i].type.empty() && i > store_id && stop != 1){
			remembered_elements = i;
			stop = 1;
		}
	}
	std::cout<<"[i] remembered id: "<<remembered_elements<<std::endl;
}

void element_alloc(){
	for(int i=0; i<MAX_ELEMENTS; i++){
		if(!elements[i].type.empty())
			active_elements = i;
	}
}

void mv_element_up(){
	if(store_id>1){
		find_previous_element();
		std::swap(elements[store_id], elements[remembered_elements]);
		if(elements[store_id].type == "heraldry"){
			buttons_elements[store_id]->setImage(elements[store_id].path.c_str());
		}
		else{
			buttons_elements[store_id]->setImage("assets/flagbuilder/gui/text_icon.png");
		}
		if(elements[remembered_elements].type == "heraldry"){
			buttons_elements[remembered_elements]->setImage(elements[remembered_elements].path.c_str());
		}
		else{
			buttons_elements[remembered_elements]->setImage("assets/flagbuilder/gui/text_icon.png");
		}
		store_id = remembered_elements;
	}
}

void mv_element_down(){
	element_alloc();
	if(store_id < MAX_ELEMENTS && store_id < active_elements){
		find_next_element();
		std::swap(elements[store_id], elements[remembered_elements]);
		if(elements[store_id].type == "heraldry"){
			buttons_elements[store_id]->setImage(elements[store_id].path.c_str());
		}
		else{
			buttons_elements[store_id]->setImage("assets/flagbuilder/gui/text_icon.png");
		}
		if(elements[remembered_elements].type == "heraldry"){
			buttons_elements[remembered_elements]->setImage(elements[remembered_elements].path.c_str());
		}
		else{
			buttons_elements[remembered_elements]->setImage("assets/flagbuilder/gui/text_icon.png");
		}
		store_id = remembered_elements;
	}
	
}

void create_element_button(std::string p){
		buttons_elements[active_elements+1] = tgui::BitmapButton::create();
		buttons_elements[active_elements+1]->setSize("100%",36);
		buttons_elements[active_elements+1]->setImage(p.c_str());
		buttons_elements[active_elements+1]->setImageScaling(0.25);
		buttons_elements[active_elements+1]->onPress(ptr_buffer, active_elements+1);
		wrapper_y += 36;
		//std::cout<<wrapper_y<<std::endl;
		wrapper_layers->add(buttons_elements[active_elements+1]);
		std::cout<<active_elements<<std::endl;
	}

void load_element_button(std::string p, int id){
		buttons_elements[id] = tgui::BitmapButton::create();
		buttons_elements[id]->setSize("100%",36);
		buttons_elements[id]->setImage(p.c_str());
		buttons_elements[id]->setImageScaling(0.25);
		buttons_elements[id]->onPress(ptr_buffer, id);
		//std::cout<<wrapper_y<<std::endl;
		wrapper_layers->add(buttons_elements[id]);
		//std::cout<<active_elements<<std::endl;
	}

void create_text(){
	std::string p;
	added_elements++;
	element_alloc();
	elements[active_elements+1].r = 255;
	elements[active_elements+1].g = 255;
	elements[active_elements+1].b = 0;
	elements[active_elements+1].scale_x = 1;
	elements[active_elements+1].scale_y = 1;
	elements[active_elements+1].element_text.setOrigin(elements[active_elements+1].element_text.getLocalBounds().width/2, elements[active_elements+1].element_text.getLocalBounds().height/2);
	elements[active_elements+1].pos_x = f_x/2;
	elements[active_elements+1].pos_y = f_y/2;
	elements[active_elements+1].element_text_size = 20;
	p = "assets/flagbuilder/gui/text_icon.png";
	
	elements[active_elements+1].type = "text";
	
	create_element_button(p);
}

void create_element(std::string p)
{
	added_elements++;
	element_alloc();
	
	elements[active_elements+1].path = p;
	elements[active_elements+1].element_texture.loadFromFile(elements[active_elements+1].path.c_str());
	elements[active_elements+1].element_sprite.setTexture(elements[active_elements+1].element_texture);
	elements[active_elements+1].r = 0;
	elements[active_elements+1].g = 255;
	elements[active_elements+1].b = 255;
	elements[active_elements+1].scale_x = 1;
	elements[active_elements+1].scale_y = 1;
	elements[active_elements+1].element_sprite.setOrigin(elements[active_elements+1].element_sprite.getLocalBounds().width/2, elements[active_elements+1].element_sprite.getLocalBounds().height/2);
	elements[active_elements+1].pos_x = f_x/2;
	elements[active_elements+1].pos_y = f_y/2;
	elements[active_elements+1].element_text_size = 9;
	elements[active_elements+1].element_str_text = "0";
	
	elements[active_elements+1].type = "heraldry";
	
	create_element_button(p);
	//std::cout<<p<<std::endl;		// debug paths
	//std::cout<<elements[active_elements+1].pos_x<<std::endl;	//debug positions 
	//std::cout<<elements[active_elements+1].pos_y<<std::endl;
}

void save_file()
{
	std::ofstream _save;
	
	// create .fbds(FlagBuilder Save) file
	_save.open(save_file_path.c_str());
	_save << f_x << "\n";
	_save << f_y << "\n";
	_save << zoom_value << "\n";
	_save << wrapper_y << "\n";
	_save << added_elements << "\n";
	for(int i=1; i<MAX_ELEMENTS; i++){
		_save << elements[i].type << "\n";
		_save << elements[i].path << "\n";
		_save << elements[i].r << "\n";
		_save << elements[i].g << "\n";
		_save << elements[i].b << "\n";
		_save << elements[i].pos_x << "\n";
		_save << elements[i].pos_y << "\n";
		_save << elements[i].scale_x << "\n";
		_save << elements[i].scale_y << "\n";
		_save << elements[i].e_rotation << "\n";
		_save << elements[i].element_str_text << "\n";
		_save << elements[i].element_text_size << "\n";
	}
	_save.close();
}

void open_file()
{
	std::ifstream _save;
	
	// load .fbds file
	_save.open(save_file_path.c_str());
	if(_save){
		_save >> f_x;
		_save >> f_y;
		_save >> zoom_value;
		_save >> wrapper_y;
		_save >> added_elements;
		for(int i=1; i<MAX_ELEMENTS; i++){
			_save >> elements[i].type;
			_save.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(_save, elements[i].path);
			_save >> elements[i].r;
			_save >> elements[i].g;
			_save >> elements[i].b;
			_save >> elements[i].pos_x;
			_save >> elements[i].pos_y;
			_save >> elements[i].scale_x;
			_save >> elements[i].scale_y;
			_save >> elements[i].e_rotation;
			_save.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(_save, elements[i].element_str_text);
			_save >> elements[i].element_text_size;
			if(elements[i].type == "heraldry")
			{
				load_element_button(elements[i].path, i);
				elements[i].element_texture.loadFromFile(elements[i].path.c_str());
				elements[i].element_sprite.setTexture(elements[i].element_texture);
				elements[i].element_sprite.setRotation(elements[i].e_rotation);
				elements[i].element_sprite.setOrigin(elements[i].element_sprite.getLocalBounds().width/2, elements[i].element_sprite.getLocalBounds().height/2);
			}
			else if(elements[i].type == "text")
			{
				load_element_button("assets/flagbuilder/gui/text_icon.png", i);
				elements[i].element_font.loadFromFile(elements[i].path);
				elements[i].element_text.setFont(elements[i].element_font);
				elements[i].element_text.setString(elements[i].element_str_text);
				elements[i].element_text.setCharacterSize(elements[i].element_text_size);
				elements[i].element_text.setRotation(elements[i].e_rotation);
			}
			else{
				elements[i] = elements[0];
			}
		}
		_save.close();
	}
	else{
		std::cout<<"[x] Couldn't open file."<<std::endl;
	}
	
}

void data_load_save()
{
	std::ifstream _data;
	std::string scandata;
	
	// load cfg
	_data.open("assets/flagbuilder/flagbd.cfg");
	if(_data){
		_data >> FBD_VERSION;
		_data >> NUM_OF_ELEMENTS;
		_data >> MAX_ELEMENTS;
		elements.resize(MAX_ELEMENTS);
		saved_tx_paths.resize(NUM_OF_ELEMENTS);
		buttons_elements.resize(MAX_ELEMENTS);
		_data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			std::getline(_data, saved_tx_paths[i].path);
		}
		_data.close();
		std::cout<<"[^] Successfully opened existing .cfg file"<<std::endl;
	}
	// create cfg
	else{
		std::ofstream _cdata;
		
		_cdata.open("assets/flagbuilder/flagbd.cfg");
		
		_cdata << "0.9\n";
		_cdata << "31\n";
		_cdata << "512\n";
		_cdata << "assets/elements/circle.png\n";
		_cdata << "assets/elements/flagmain.png\n";
		_cdata << "assets/elements/11.png\n";
		_cdata << "assets/elements/12.png\n";
		_cdata << "assets/elements/13.png\n";
		_cdata << "assets/elements/14.png\n";
		_cdata << "assets/elements/15.png\n";
		_cdata << "assets/elements/16.png\n";
		_cdata << "assets/elements/17.png\n";
		_cdata << "assets/elements/18.png\n";
		_cdata << "assets/elements/19.png\n";
		_cdata << "assets/elements/9.png\n";
		_cdata << "assets/elements/8.png\n";
		_cdata << "assets/elements/10.png\n";
		_cdata << "assets/elements/cross1.png\n";
		_cdata << "assets/elements/cross2.png\n";
		_cdata << "assets/elements/saltire.png\n";
		_cdata << "assets/elements/saltire1.png\n";
		_cdata << "assets/elements/saltire2.png\n";
		_cdata << "assets/elements/saltire3.png\n";
		_cdata << "assets/elements/a.png\n";
		_cdata << "assets/elements/b.png\n";
		_cdata << "assets/elements/sickle.png\n";
		_cdata << "assets/elements/hammer.png\n";
		_cdata << "assets/elements/laurel.png\n";
		_cdata << "assets/elements/gear.png\n";
		_cdata << "assets/elements/fasces.png\n";
		_cdata << "assets/elements/sword.png\n";
		_cdata << "assets/elements/1.png\n";
		_cdata << "assets/elements/2.png\n";
		_cdata << "assets/elements/5.png\n";
		
		_cdata.close();
		
		_data.open("assets/flagbuilder/flagbd.cfg");
		_data >> FBD_VERSION;
		_data >> NUM_OF_ELEMENTS;
		_data >> MAX_ELEMENTS;
		elements.resize(MAX_ELEMENTS);
		saved_tx_paths.resize(NUM_OF_ELEMENTS);
		buttons_elements.resize(MAX_ELEMENTS);
		_data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			std::getline(_data, saved_tx_paths[i].path);
		}
		_data.close();
		std::cout<<"[^] Successfully created and opened .cfg file"<<std::endl;
	}
	//update cfg
	if(FBD_VERSION != "0.9"){
		FBD_VERSION = "0.9";
		
		std::ofstream _udata;
		_udata.open("assets/flagbuilder/flagbd.cfg");
		
		_udata << FBD_VERSION << "\n";
		_udata << NUM_OF_ELEMENTS << "\n";
		_udata << MAX_ELEMENTS << "\n";
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			_udata << saved_tx_paths[i].path+"\n";
		}
		_udata.close();
		std::cout<<"[^] Updated .cfg file"<<std::endl;
	}
	
}

int main()
{
	data_load_save();
	sf::RenderWindow window{{sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height-64}, "FlagBuilder "+FBD_VERSION};
	gui.setTarget(window);
	tgui::Theme::setDefault("assets/flagbuilder/gui/Black.txt");
	
	tgui::Theme theme_main("assets/flagbuilder/gui/Black.txt");
	
	rdbutton_font_r->setRenderer(theme_main.getRenderer("RadioButton"));
	rdbutton_font_b->setRenderer(theme_main.getRenderer("RadioButton"));
	rdbutton_font_i->setRenderer(theme_main.getRenderer("RadioButton"));
	rdbutton_font_u->setRenderer(theme_main.getRenderer("RadioButton"));
	rdbutton_font_st->setRenderer(theme_main.getRenderer("RadioButton"));
	
	x = sf::VideoMode::getDesktopMode().width;
	y = sf::VideoMode::getDesktopMode().height-64;
	
	window.setFramerateLimit(60);
	//////////////////////////////////////////////
	//	Solves issues with high CPU usage,		//
	//	if usage is still high, try lowering or	//
	//	disabling the FPS limit.				//
	//	Also try checking your GPU drivers.		//
	//////////////////////////////////////////////
	
	sf::Texture logo_texture;
	
	logo_texture.loadFromFile("assets/flagbuilder/gui/Flagbd_logo.png");
	tgui::Picture::Ptr logo_sprite = tgui::Picture::create(logo_texture);
	
	/*
	 * 				SETUP INTERFACE 	&	WINDOWS
	*/
	
	
	auto menu_file_primary = tgui::MenuBar::create();
	auto lower_panel = tgui::Panel::create();

	auto zoom_slider = tgui::Slider::create();
	
	auto window_about = tgui::ChildWindow::create();
	auto l_about = tgui::TextArea::create();
	
	auto window_createfile = tgui::ChildWindow::create();
	auto window_exportfile = tgui::ChildWindow::create();
	auto window_savefile = tgui::ChildWindow::create();
	auto window_openfile = tgui::ChildWindow::create();
	auto window_addheraldry = tgui::ChildWindow::create();
	
	auto button_exportfile = tgui::Button::create();
	
	auto rdbutton_png = tgui::RadioButton::create();
	auto rdbutton_tga = tgui::RadioButton::create();
	auto rdbutton_jpg = tgui::RadioButton::create();
	auto rdbutton_bmp = tgui::RadioButton::create();
	
	auto l_extension = tgui::Label::create();
	auto l_path = tgui::Label::create();
	auto l_spath = tgui::Label::create();
	auto l_opath = tgui::Label::create();
	auto l_hpath = tgui::Label::create();
	auto ebox_path = tgui::EditBox::create();
	auto ebox_spath = tgui::EditBox::create();
	auto ebox_opath = tgui::EditBox::create();
	auto ebox_hpath = tgui::EditBox::create();
	
	auto button_createfile = tgui::Button::create();
	auto button_savefile = tgui::Button::create();
	auto button_loadfile = tgui::Button::create();
	auto cw_button_addheraldry = tgui::Button::create();
	
	//int file_width, file_height;
	auto filew_input = tgui::EditBox::create();
	auto fileh_input = tgui::EditBox::create();
	auto wcf_label_w = tgui::Label::create();
	auto wcf_label_h = tgui::Label::create();
	
	//menus
	menu_file_primary->addMenu("File");
	menu_file_primary->addMenuItem({"File", "Create..."});
	menu_file_primary->addMenuItem({"File", "Open"});
	menu_file_primary->addMenuItem({"File", "Save..."});
	menu_file_primary->addMenuItem({"File", "Export..."});
	menu_file_primary->addMenuItem({"File", "Close"});
	menu_file_primary->addMenu("View");
	menu_file_primary->addMenuItem({"View", "Smooth mode on/off"});
	menu_file_primary->addMenu("Help");
	menu_file_primary->addMenuItem({"Help", "About"});
	menu_file_primary->setSize(sf::VideoMode::getDesktopMode().width,18);
	menu_file_primary->setPosition(0,0);
	lower_panel->setSize(sf::VideoMode::getDesktopMode().width,18);
	lower_panel->setPosition(0,"98%");
	zoom_slider->setSize(sf::VideoMode::getDesktopMode().width/16, 18);
	zoom_slider->setPosition(0,0);
	
	zoom_slider->setStep(0.1);
	zoom_slider->setMinimum(0.1);
	zoom_slider->setMaximum(1.25);
	zoom_slider->setValue(1);
	
	
	//windows
	window_about->setSize("30%","42%");
	window_about->setTitle("About FlagBuilder");
	window_about->setResizable(false);
	
	window_createfile->setSize("30%","13%");
	window_createfile->setTitle("Create file...");
	window_createfile->setResizable(false);
	
	window_exportfile->setSize("30%", "30%");
	window_exportfile->setTitle("Export file...");
	window_exportfile->setResizable(false);
	
	window_savefile->setSize("30%", "30%");
	window_savefile->setTitle("Save file...");
	window_savefile->setResizable(false);
	
	window_openfile->setSize("30%", "30%");
	window_openfile->setTitle("Open file...");
	window_openfile->setResizable(false);
	
	window_addheraldry->setSize("30%", "30%");
	window_addheraldry->setTitle("Add heraldry...");
	window_addheraldry->setResizable(false);
	
	wcf_label_w->setSize("22%","45%");
	wcf_label_h->setSize("25%","45%"); 
	wcf_label_w->setPosition("2%","1%");
	wcf_label_h->setPosition("2%","30%");
	wcf_label_w->setTextSize(20);
	wcf_label_h->setTextSize(20);
	wcf_label_w->setText("Width");
	wcf_label_h->setText("Height");
	
	button_createfile->setSize("40%","100%");
	button_createfile->setText("Create");
	button_createfile->setPosition("60%","1%");
	
	filew_input->setSize("14%","30%");
	fileh_input->setSize("14%","30%");
	filew_input->setPosition("22%","1%");
	fileh_input->setPosition("22%","31%");
	filew_input->setMaximumCharacters(4);
	fileh_input->setMaximumCharacters(4);
	filew_input->setInputValidator("[0-9]*");
	fileh_input->setInputValidator("[0-9]*");
	filew_input->setText("300");
	fileh_input->setText("200");
	
	l_extension->setSize("20%","10%");
	l_extension->setPosition("5%","2%");
	l_extension->setText("Extension:");
	l_path->setSize("32%","10%");
	l_path->setPosition("5%","15%");
	l_path->setText("Path(w/ filename):");
	l_spath->setSize("32%","10%");
	l_spath->setPosition("5%","15%");
	l_spath->setText("Path(w/ filename):");
	l_opath->setSize("32%","10%");
	l_opath->setPosition("5%","15%");
	l_opath->setText("Path(w/ filename):");
	l_hpath->setText("Path(w/ filename):");
	l_hpath->setSize("32%","10%");
	l_hpath->setPosition("5%","15%");
	
	ebox_path->setSize("56%","10%");
	ebox_path->setPosition("35%","15%");
	ebox_path->setText("flag");
	ebox_spath->setSize("56%","10%");
	ebox_spath->setPosition("35%","15%");
	ebox_spath->setText("flag");
	ebox_opath->setSize("56%","10%");
	ebox_opath->setPosition("35%","15%");
	ebox_opath->setText("flag.fbds");
	ebox_hpath->setSize("56%","10%");
	ebox_hpath->setPosition("35%","15%");
	ebox_hpath->setText("");
	
	rdbutton_png->setSize("6%","10%");
	rdbutton_jpg->setSize("6%","10%");
	rdbutton_tga->setSize("6%","10%");
	rdbutton_bmp->setSize("6%","10%");
	rdbutton_png->setText(".png");
	rdbutton_jpg->setText(".jpg");
	rdbutton_tga->setText(".tga");
	rdbutton_bmp->setText(".bmp");
	rdbutton_png->setPosition("31%","2%");
	rdbutton_jpg->setPosition("46%","2%");
	rdbutton_tga->setPosition("60%","2%");
	rdbutton_bmp->setPosition("74%","2%");
	
	rdbutton_png->onCheck([&]{ file_extension = ".png"; });
	rdbutton_jpg->onCheck([&]{ file_extension = ".jpg"; });
	rdbutton_tga->onCheck([&]{ file_extension = ".tga"; });
	rdbutton_bmp->onCheck([&]{ file_extension = ".bmp"; });
	
	button_exportfile->setSize("25%","25%");
	button_exportfile->setText("Export");
	button_exportfile->setPosition("75%","73%");
	button_exportfile->onPress([&]{ tgui::String filepath = ebox_path->getText(); rendered_texture.getTexture()->copyToImage().saveToFile(filepath.toStdString()+file_extension); });
	
	button_savefile->setSize("25%","25%");
	button_savefile->setText("Save");
	button_savefile->setPosition("75%","73%");
	button_savefile->onPress([&](){ tgui::String filepath = ebox_spath->getText(); save_file_path = filepath.toStdString()+".fbds"; save_file(); });
	
	button_loadfile->setSize("25%","25%");
	button_loadfile->setText("Open");
	button_loadfile->setPosition("75%","73%");
	
	cw_button_addheraldry->setSize("25%","25%");
	cw_button_addheraldry->setText("Add");
	cw_button_addheraldry->setPosition("75%","73%");
	
	logo_sprite->setPosition("25%",0);
	l_about->setSize("100%", "35%");
	l_about->setPosition(0,"34%");
	l_about->setReadOnly(true);
	l_about->setText("FlagBuilder version "+FBD_VERSION+", an open-source flag-making tool written by Pravetz.\n FlagBuilder(or Flagbd) is distributed under GNU GPLv3 license.\n More information:\nhttps://github.com/Pravetz/Flagbd");
	window_about->add(logo_sprite);
	window_about->add(l_about);
	
	window_createfile->add(wcf_label_w);
	window_createfile->add(wcf_label_h);
	window_createfile->add(filew_input);
	window_createfile->add(fileh_input);
	window_createfile->add(button_createfile);
	
	window_exportfile->add(l_extension);
	window_exportfile->add(l_path);
	window_exportfile->add(ebox_path);
	window_exportfile->add(button_exportfile);
	window_exportfile->add(rdbutton_png);
	window_exportfile->add(rdbutton_jpg);
	window_exportfile->add(rdbutton_tga);
	window_exportfile->add(rdbutton_bmp);
	
	window_savefile->add(l_spath);
	window_savefile->add(ebox_spath);
	window_savefile->add(button_savefile);
	
	window_openfile->add(l_opath);
	window_openfile->add(ebox_opath);
	window_openfile->add(button_loadfile);
	
	window_addheraldry->add(l_hpath);
	window_addheraldry->add(ebox_hpath);
	window_addheraldry->add(cw_button_addheraldry);
	
	auto panel_heraldry = tgui::ScrollablePanel::create();
	auto wrapper_heraldry = tgui::HorizontalWrap::create();
	tgui::BitmapButton::Ptr buttons_heraldry[NUM_OF_ELEMENTS];
	
	auto edit_panel = tgui::Panel::create();
	auto wrapper_editb = tgui::HorizontalWrap::create();
	auto properties_panel = tgui::ScrollablePanel::create();
	auto layer_panel = tgui::ScrollablePanel::create();
	
	auto button_addtext = tgui::Button::create();
	auto button_addheraldry = tgui::BitmapButton::create();
	
	auto label_font_path = tgui::Label::create();
	auto label_str_text = tgui::Label::create();
	auto label_str_size = tgui::Label::create();
	
	rdbutton_font_r->setChecked(true);
	rdbutton_font_r->setSize("10%","10%");
	rdbutton_font_b->setSize("10%","10%");
	rdbutton_font_i->setSize("10%","10%");
	rdbutton_font_u->setSize("10%","10%");
	rdbutton_font_st->setSize("10%","10%");
	rdbutton_font_r->setPosition("5%","106%");
	rdbutton_font_b->setPosition("5%","116%");
	rdbutton_font_i->setPosition("5%","126%");
	rdbutton_font_u->setPosition("5%","136%");
	rdbutton_font_st->setPosition("5%","146%");
	rdbutton_font_r->setText("Regular");
	rdbutton_font_b->setText("Bold");
	rdbutton_font_i->setText("Italic");
	rdbutton_font_u->setText("Underlined");
	rdbutton_font_st->setText("StrikeThrough");
	
	rdbutton_font_r->onCheck([&](){ elements[store_id].element_text.setStyle(sf::Text::Regular); });
	rdbutton_font_b->onCheck([&](){ elements[store_id].element_text.setStyle(sf::Text::Bold); });
	rdbutton_font_i->onCheck([&](){ elements[store_id].element_text.setStyle(sf::Text::Italic); });
	rdbutton_font_u->onCheck([&](){ elements[store_id].element_text.setStyle(sf::Text::Underlined); });
	rdbutton_font_st->onCheck([&](){ elements[store_id].element_text.setStyle(sf::Text::StrikeThrough); });
	
	auto del_element_button = tgui::BitmapButton::create();
	del_element_button->setSize("6%","5%");
	del_element_button->setPosition("77%","0.65%");
	auto element_mv_up_button = tgui::BitmapButton::create();
	element_mv_up_button->setSize("6%","5%");
	element_mv_up_button->setPosition("84%","0.65%");
	auto element_mv_down_button = tgui::BitmapButton::create();
	element_mv_down_button->setSize("6%","5%");
	element_mv_down_button->setPosition("91%","0.65%");
	
	button_addtext->setSize("3%","4%");
	button_addtext->setText("Aa");
	button_addtext->setPosition(0,"46%");
	
	button_addheraldry->setSize("3%","4%");
	button_addheraldry->setImage("assets/flagbuilder/gui/icon_add_heraldry.png");
	button_addheraldry->setPosition("3%","46%");
	
	edit_panel->setPosition(0, "2.6%");
	edit_panel->setSize("20%", "43%");
	properties_panel->setPosition("80%", "2.6%");
	properties_panel->setSize("20%", "43%");
	panel_heraldry->setPosition(0, "50%");
	panel_heraldry->setSize("20%", "47%");
	layer_panel->setPosition("80%", "46%");
	layer_panel->setSize("20%", "50%");
	
	edit_panel->getRenderer()->setBackgroundColor(sf::Color(55,55,55));
	properties_panel->getRenderer()->setBackgroundColor(sf::Color(55,55,55));
	panel_heraldry->getRenderer()->setBackgroundColor(sf::Color(55,55,55));
	layer_panel->getRenderer()->setBackgroundColor(sf::Color(55,55,55));
	
	wrapper_heraldry->setSize("100%", (NUM_OF_ELEMENTS*32)/2);
	
	selected_e_name->setSize("46%","6%");
	selected_e_name->setPosition("0.7%","0.9%");
	wrapper_editb->setSize("100%","100%");
	
	label_font_path->setSize("30%","6%");
	label_font_path->setText("Font path:");
	label_str_text->setSize("15%","6%");
	label_str_text->setText("Text:");
	ebox_font_path->setSize("62%","6%");
	ebox_str_text->setSize("62%","6%");
	ebox_str_size->setSize("62%","6%");
	ebox_str_size->setText("9");
	ebox_str_size->setMaximumCharacters(3);
	fileh_input->setInputValidator("[0-9]*");
	label_font_path->setPosition("5%","82%");
	ebox_font_path->setPosition("31%","82%");
	label_str_text->setPosition("5%","90%");
	ebox_str_text->setPosition("31%","90%");
	ebox_str_size->setPosition("31%","98%");
	label_str_size->setSize("30%","6%");
	label_str_size->setText("Text size:");
	label_str_size->setPosition("5%","98%");
	
	//	edit panel buttons
	tgui::BitmapButton::Ptr button_center_element = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_color = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_color = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_scale = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_scale = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_rotation = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_rotation = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr copy_position = tgui::BitmapButton::create();
	tgui::BitmapButton::Ptr paste_position = tgui::BitmapButton::create();
	
	del_element_button->setImage("assets/flagbuilder/gui/delete_element_icon.png");
	element_mv_up_button->setImage("assets/flagbuilder/gui/e_up_icon.png");
	element_mv_down_button->setImage("assets/flagbuilder/gui/e_down_icon.png");
	button_center_element->setImage("assets/flagbuilder/gui/icon_center_element.png");
	copy_color->setImage("assets/flagbuilder/gui/copy_color_icon.png");
	paste_color->setImage("assets/flagbuilder/gui/paste_color_icon.png");
	copy_scale->setImage("assets/flagbuilder/gui/copy_scale_icon.png");
	paste_scale->setImage("assets/flagbuilder/gui/paste_scale_icon.png");
	copy_rotation->setImage("assets/flagbuilder/gui/copy_rotation_icon.png");
	paste_rotation->setImage("assets/flagbuilder/gui/paste_rotation_icon.png");
	copy_position->setImage("assets/flagbuilder/gui/copy_position_icon.png");
	paste_position->setImage("assets/flagbuilder/gui/paste_position_icon.png");
	
	button_center_element->setSize("12%","11%");
	copy_color->setSize("12%","11%");
	paste_color->setSize("12%","11%");
	copy_scale->setSize("12%","11%");
	paste_scale->setSize("12%","11%");
	copy_rotation->setSize("12%","11%");
	paste_rotation->setSize("12%","11%");
	copy_position->setSize("12%","11%");
	paste_position->setSize("12%","11%");
	
	del_element_button->onPress([&](){ elements[store_id] = elements[0]; wrapper_layers->remove(buttons_elements[store_id]); added_elements--; wrapper_y -= 32; store_id = 0; active_elements = 0; std::cout<<added_elements<<" "<<wrapper_y<<std::endl; });
	button_addtext->onPress(create_text);
	button_addheraldry->onPress([&]() {gui.add(window_addheraldry);});
	element_mv_up_button->onPress(mv_element_up);
	element_mv_down_button->onPress(mv_element_down);
	
	button_center_element->onPress([&](){ elements[store_id].pos_x = f_x/2; elements[store_id].pos_y = f_y/2; });
	copy_color->onPress([&](){ saved_r = elements[store_id].r; saved_g = elements[store_id].g; saved_b = elements[store_id].b; });
	paste_color->onPress([&](){ elements[store_id].r = saved_r; elements[store_id].g = saved_g; elements[store_id].b = saved_b; });
	copy_scale->onPress([&](){ saved_scale_x = elements[store_id].scale_x; saved_scale_y = elements[store_id].scale_y; });
	paste_scale->onPress([&](){ elements[store_id].scale_x = saved_scale_x; elements[store_id].scale_y = saved_scale_y; });
	copy_rotation->onPress([&](){ saved_rotation = elements[store_id].element_sprite.getRotation(); });
	paste_rotation->onPress([&](){ elements[store_id].element_sprite.setRotation(saved_rotation); });
	copy_position->onPress([&](){ saved_pos_x = elements[store_id].pos_x; saved_pos_y = elements[store_id].pos_y; });
	paste_position->onPress([&](){ elements[store_id].pos_x = saved_pos_x; elements[store_id].pos_y = saved_pos_y; });
	
	wrapper_editb->add(button_center_element);
	wrapper_editb->add(copy_color);
	wrapper_editb->add(paste_color);
	wrapper_editb->add(copy_scale);
	wrapper_editb->add(paste_scale);
	wrapper_editb->add(copy_rotation);
	wrapper_editb->add(paste_rotation);
	wrapper_editb->add(copy_position);
	wrapper_editb->add(paste_position);
	edit_panel->add(wrapper_editb);
	
	// sliders, labels, editboxes
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
	
	rotation->setMinimum(0);
	rotation->setMaximum(360);
	
	slider_r->setSize("86%","5%");
	slider_g->setSize("86%","5%");
	slider_b->setSize("86%","5%");
	slider_r->setPosition("11%","7%");
	slider_g->setPosition("11%","15%");
	slider_b->setPosition("11%","23%");
	
	ebox_pos_x->setSize("30%","5%");
	ebox_pos_y->setSize("30%","5%");
	ebox_scale_x->setSize("30%","5%");
	ebox_scale_y->setSize("30%","5%");
	ebox_pos_x->setPosition("30%","49%");
	ebox_pos_y->setPosition("30%","58%");
	ebox_scale_x->setPosition("30%","31%");
	ebox_scale_y->setPosition("30%","40%");
	
	ebox_pos_x->setInputValidator("[0-9]*");
	ebox_pos_y->setInputValidator("[0-9]*");
	ebox_scale_x->setInputValidator("[0.0-9.0]*(-|+)");
	ebox_scale_y->setInputValidator("[0.0-9.0]*(-|+)");
	ebox_pos_x->setMaximumCharacters(4);
	ebox_pos_y->setMaximumCharacters(4);
	ebox_scale_x->setMaximumCharacters(4);
	ebox_scale_y->setMaximumCharacters(4);
	
	rotation->setSize("70%","3%");
	rotation->setPosition("31%","69%");
	
	label_r->setSize("10%","10%");
	label_g->setSize("10%","10%");
	label_b->setSize("10%","10%");
	label_r->setPosition("5%","8%");
	label_g->setPosition("5%","16%");
	label_b->setPosition("5%","24%");
	
	label_rgb->setSize("40%","5%");
	label_rgb->setPosition("63%","31%");
	label_rotated->setSize("34%", "5%");
	label_rotated->setPosition("64%", "73%");
	label_rgb->setRenderer(theme_main.getRenderer("Label"));
	label_rotated->setRenderer(theme_main.getRenderer("Label"));
	
	label_r->setText("R:");
	label_g->setText("G:");
	label_b->setText("B:");
	
	label_scale_x->setText("Scale x:");
	label_scale_y->setText("Scale y:");
	
	label_rotate->setText("Rotate:");
	label_pos_x->setText("Pos x:");
	label_pos_y->setText("Pos y:");
	label_pos_x->setSize("20%","7%");
	label_pos_y->setSize("20%","7%");
	label_pos_x->setPosition("5%","50%");
	label_pos_y->setPosition("5%","58%");
	
	label_rotate->setSize("25%","7%");
	label_rotate->setPosition("5%","68%");
	label_scale_x->setSize("25%","7%");
	label_scale_y->setSize("25%","7%");
	label_scale_x->setPosition("5%","33%");
	label_scale_y->setPosition("5%","41%");
	
	properties_panel->add(del_element_button);
	properties_panel->add(element_mv_up_button);
	properties_panel->add(element_mv_down_button);
	properties_panel->add(label_r);
	properties_panel->add(label_g);
	properties_panel->add(label_b);
	
	properties_panel->add(label_pos_x);
	properties_panel->add(label_pos_y);
	
	properties_panel->add(slider_r);
	properties_panel->add(slider_g);
	properties_panel->add(slider_b);
	
	properties_panel->add(ebox_pos_x);
	properties_panel->add(ebox_pos_y);
	
	selected_e_name->setRenderer(theme_main.getRenderer("Label"));
	properties_panel->add(selected_e_name);
	properties_panel->add(label_rgb);
	
	properties_panel->add(ebox_scale_x);
	properties_panel->add(ebox_scale_y);
	properties_panel->add(label_scale_x);
	properties_panel->add(label_scale_y);
	properties_panel->add(rotation);
	properties_panel->add(label_rotate);
	properties_panel->add(label_rotated);
	properties_panel->add(label_font_path);
	properties_panel->add(ebox_font_path);
	properties_panel->add(label_str_text);
	properties_panel->add(ebox_str_text);
	properties_panel->add(label_str_size);
	properties_panel->add(ebox_str_size);
	properties_panel->add(rdbutton_font_r);
	properties_panel->add(rdbutton_font_b);
	properties_panel->add(rdbutton_font_i);
	properties_panel->add(rdbutton_font_u);
	properties_panel->add(rdbutton_font_st);
	
	//signals
	menu_file_primary->connectMenuItem({"Help", "About"}, [&](){ gui.add(window_about); });
	menu_file_primary->connectMenuItem({"File", "Create..."}, [&](){ gui.add(window_createfile); });
	menu_file_primary->connectMenuItem({"File", "Export..."}, [&](){ gui.add(window_exportfile); });
	menu_file_primary->connectMenuItem({"File", "Save..."}, [&](){ gui.add(window_savefile); });
	menu_file_primary->connectMenuItem({"File", "Open"}, [&](){ gui.add(window_openfile); });
	menu_file_primary->connectMenuItem({"View", "Smooth mode on/off"}, [&]{
		if(smooth_enabled){
			smooth_enabled = false;
			for(int i=0; i<MAX_ELEMENTS; i++){
				elements[i].element_texture.setSmooth(false);
			}
		}
		else{
			smooth_enabled = true;
			for(int i=0; i<MAX_ELEMENTS; i++){
				elements[i].element_texture.setSmooth(true);
			}
		}
	});
	
	//add interface buttons, wrappers
	gui.add(menu_file_primary);
	lower_panel->add(zoom_slider);
	gui.add(lower_panel);
	
	display_view.setSize(x,y);
	display_view.setCenter(x/2, y/2);
	display_view.zoom(zoom_value);
	
	panel_heraldry->add(wrapper_heraldry);
	layer_panel->add(wrapper_layers);
	
	zoom_slider->onValueChange([&]{ zoom_value = zoom_slider->getValue(); display_view.zoom(zoom_value); window.setView(display_view); std::cout<<"[i] zoom_value: "<<zoom_value<<std::endl; });
	button_loadfile->onPress([&](){
		tgui::String filepath = ebox_opath->getText();
		save_file_path = filepath.toStdString();
		open_file();
		window.setView(display_view); 
		tgui::String s_w = filew_input->getText();
		tgui::String s_h = fileh_input->getText(); 
		if(!texture.create(f_x, f_y)) 
			{ return EXIT_FAILURE; } 
		rendered_texture.setTexture(texture.getTexture(), true); 
		rendered_texture.setOrigin(rendered_texture.getLocalBounds().width/2, 
		rendered_texture.getLocalBounds().height/2); rendered_texture.setPosition(x/2,y/2); 
		FILE_OPEN = true; gui.add(panel_heraldry); gui.add(edit_panel); 
		gui.add(properties_panel); 
		gui.add(layer_panel); 
		gui.add(button_addtext);
		gui.add(button_addheraldry);
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			buttons_heraldry[i] = tgui::BitmapButton::create();
			buttons_heraldry[i]->setSize("13%",36);
			buttons_heraldry[i]->setImage(saved_tx_paths[i].path.c_str());
			buttons_heraldry[i]->setImageScaling(0.25);
			wrapper_heraldry->add(buttons_heraldry[i]);
			buttons_heraldry[i]->onPress(&create_element, saved_tx_paths[i].path.c_str());
		
			//std::cout<<"[i] Heraldry: "<< i <<std::endl; 					// for debugging
		}
		gui.remove(window_openfile);
	});
	button_createfile->onPress([&]{
		window.setView(display_view); 
		tgui::String s_w = filew_input->getText();
		tgui::String s_h = fileh_input->getText(); 
		f_x = s_w.toInt();
		f_y = s_h.toInt();
		if(!texture.create(s_w.toInt(), s_h.toInt())) 
			{ return EXIT_FAILURE; } 
		rendered_texture.setTexture(texture.getTexture(), true); 
		rendered_texture.setOrigin(rendered_texture.getLocalBounds().width/2, 
		rendered_texture.getLocalBounds().height/2); rendered_texture.setPosition(x/2,y/2); 
		FILE_OPEN = true; gui.add(panel_heraldry); gui.add(edit_panel); 
		gui.add(properties_panel); 
		gui.add(layer_panel); 
		gui.add(button_addtext);
		gui.add(button_addheraldry);
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			buttons_heraldry[i] = tgui::BitmapButton::create();
			buttons_heraldry[i]->setSize("13%",36);
			buttons_heraldry[i]->setImage(saved_tx_paths[i].path.c_str());
			buttons_heraldry[i]->setImageScaling(0.25);
			wrapper_heraldry->add(buttons_heraldry[i]);
			buttons_heraldry[i]->onPress(&create_element, saved_tx_paths[i].path.c_str());
		
			//std::cout<<"[i] Heraldry: "<< i <<std::endl; 					// for debugging
		}
		gui.remove(window_createfile);
	});
	
	menu_file_primary->connectMenuItem({"File", "Close"}, [&](){ 
		for(int i=0; i<NUM_OF_ELEMENTS; i++){
			wrapper_heraldry->remove(buttons_heraldry[i]);
		}
		for(int i=1; i<MAX_ELEMENTS; i++){
			elements[i] = elements[0];
			wrapper_layers->remove(buttons_elements[i]);
		}
		added_elements = 0;
		active_elements = 0;
		store_id = 0;
		wrapper_y = 36;
		zoom_value=1;
		f_x = 0;
		f_y = 0;
		
		gui.remove(panel_heraldry); 
		gui.remove(edit_panel); 
		gui.remove(properties_panel); 
		gui.remove(layer_panel); 
		gui.remove(button_addtext);
		gui.remove(button_addheraldry);
		window.clear(sf::Color(55,55,55));
		FILE_OPEN = false;
	});
	
	cw_button_addheraldry->onPress([&](){
		sf::Texture validate_texture;
		
		tgui::String filepath = ebox_hpath->getText();
		std::string load_heraldry_path = filepath.toStdString();
		
		if(validate_texture.loadFromFile(load_heraldry_path.c_str())){
			NUM_OF_ELEMENTS++;
			//std::cout<<NUM_OF_ELEMENTS<<std::endl;		//for debugging
			saved_tx_paths.resize(NUM_OF_ELEMENTS);
			saved_tx_paths[NUM_OF_ELEMENTS-1].path = load_heraldry_path;
			//std::cout<<saved_tx_paths[NUM_OF_ELEMENTS-1].path<<std::endl;		//for debugging
			//add buttons
			buttons_heraldry[NUM_OF_ELEMENTS-1] = tgui::BitmapButton::create();
			buttons_heraldry[NUM_OF_ELEMENTS-1]->setSize("13%",36);
			buttons_heraldry[NUM_OF_ELEMENTS-1]->setImage(saved_tx_paths[NUM_OF_ELEMENTS-1].path.c_str());
			buttons_heraldry[NUM_OF_ELEMENTS-1]->setImageScaling(0.25);
			wrapper_heraldry->add(buttons_heraldry[NUM_OF_ELEMENTS-1]);
			buttons_heraldry[NUM_OF_ELEMENTS-1]->onPress(&create_element, saved_tx_paths[NUM_OF_ELEMENTS-1].path.c_str());
			
			// update flagbd.cfg
			std::ofstream _udata;
			_udata.open("assets/flagbuilder/flagbd.cfg");
			
			_udata << FBD_VERSION << "\n";
			_udata << NUM_OF_ELEMENTS << "\n";
			_udata << MAX_ELEMENTS << "\n";
			for(int i=0; i<NUM_OF_ELEMENTS; i++){
				_udata << saved_tx_paths[i].path+"\n";
			}
			_udata.close();
			std::cout<<"[^] Updated .cfg file"<<std::endl;
			
			gui.remove(window_addheraldry);
		}
	});
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			gui.handleEvent(event);
	
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		
		if(FILE_OPEN){
			menu_file_primary->setMenuItemEnabled({"File", "Save..."}, true);
			menu_file_primary->setMenuItemEnabled({"File", "Export..."}, true);
			menu_file_primary->setMenuItemEnabled({"File", "Create..."}, false);
			menu_file_primary->setMenuItemEnabled({"File", "Open"}, false);
			menu_file_primary->setMenuItemEnabled({"View", "Smooth mode on/off"}, true);
			menu_file_primary->setMenuItemEnabled({"File", "Close"}, true);
			wrapper_layers->setSize("100%", wrapper_y);
			rendered_texture.setTexture(texture.getTexture(), true); 
			texture.clear(sf::Color::White);
			texture.display();
			for(int i=0; i<MAX_ELEMENTS; i++){
				if(!elements[i].path.empty()){
					if(elements[i].type == "heraldry"){
						elements[i].element_texture.loadFromFile(elements[i].path.c_str());
					}
					elements[i].element_sprite.setTexture(elements[i].element_texture);
					elements[i].element_sprite.setColor(sf::Color(elements[i].r, elements[i].g, elements[i].b));
					elements[i].element_sprite.setPosition(elements[i].pos_x, elements[i].pos_y);
					elements[i].element_sprite.setScale(elements[i].scale_x, elements[i].scale_y);
					elements[i].element_text.setFillColor(sf::Color(elements[i].r, elements[i].g, elements[i].b));
					elements[i].element_text.setPosition(elements[i].pos_x, elements[i].pos_y);
					elements[i].element_text.setScale(elements[i].scale_x, elements[i].scale_y);
				}
			}
			for(int i=0; i<MAX_ELEMENTS; i++){
				texture.draw(elements[i].element_sprite);
				texture.draw(elements[i].element_text);
			}
			if(elements[store_id].type == "heraldry") { label_rotated->setText("("+std::to_string(elements[store_id].element_sprite.getRotation())+")"); }
			else { label_rotated->setText("("+std::to_string(elements[store_id].element_text.getRotation())+")"); }
			label_rgb->setText("("+std::to_string(elements[store_id].r)+";"+std::to_string(elements[store_id].g)+";"+std::to_string(elements[store_id].b)+")");
			if(store_id>=1){
				del_element_button->setEnabled(true);
				element_mv_up_button->setEnabled(true);
				element_mv_down_button->setEnabled(true);
			}
			else{
				del_element_button->setEnabled(false);
				element_mv_up_button->setEnabled(false);
				element_mv_down_button->setEnabled(false);
			}
		}
		else{
			menu_file_primary->setMenuItemEnabled({"File", "Save..."}, false);
			menu_file_primary->setMenuItemEnabled({"View", "Smooth mode on/off"}, false);
			menu_file_primary->setMenuItemEnabled({"File", "Export..."}, false);
			menu_file_primary->setMenuItemEnabled({"File", "Create..."}, true);
			menu_file_primary->setMenuItemEnabled({"File", "Open"}, true);
			menu_file_primary->setMenuItemEnabled({"File", "Close"}, false);
		}
	
		window.clear(sf::Color(55,55,55));
		if(FILE_OPEN){
			window.draw(rendered_texture);
		}
		gui.draw();
		window.display();
	}
	return 0;
}
