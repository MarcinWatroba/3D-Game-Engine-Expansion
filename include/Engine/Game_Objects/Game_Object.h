#pragma once
#include <string>
#include <map>
#include <glm\glm.hpp>
#include <vector>

class Component;
class Shader;
class Texture;

class Game_Object
{
protected:
	//Name of the object
	std::string s_Name;

	std::string s_PrefabName;

	//Components
	std::map<std::string, Component*> mipo_Components;

	//Children of the object
	std::map<std::string, Game_Object*> mspo_Children;
	std::vector<std::string> vs_ChildrenNames;

	std::string s_Tag;

	std::string s_ObjectType;

	//Parent
	Game_Object* po_Parent;

	//Check if container
	bool b_IsContainer;

	int i_ObjectID;

	//Status of rendering
	bool b_RenderStatus;

	virtual glm::mat4 get_ParentMatrix();

	//
	bool b_toDelete;
	glm::vec2 v2_Tiling;

public:
	Game_Object();
	Game_Object(const Game_Object &p_NewObject_In);
	virtual void add_Component(std::string s_Name_In, Component* p_Component_In) = 0;
	virtual void add_Texture(std::string s_Name_In, Texture* p_Texture_In) = 0;
	virtual void set_Tiles(glm::vec2 v2_Tiles_In) = 0;
	virtual void update() = 0;
	virtual void force_Update() = 0;
	virtual void renderDepth(Shader* p_Shader_In) = 0;
	virtual void render(Shader* p_Shader_In) = 0;
	virtual void render(Shader* p_Shader_In, bool skybox) = 0;
	virtual void clean_Up() = 0;

	bool get_ToDelete();// get whether object should be deleted
	void set_ToDelete();// set that the object should be deleted

	void set_Name(std::string s_Name_In);
	void set_ObjectType(std::string s_Type_In);
	std::string get_Name();
	void add_ChildName(std::string s_Name_In);
	std::vector<std::string>& get_ChildrenNames();
	void set_RenderStatus(bool b_RenderStatus_In);
	void set_Prefab(std::string s_Name_In);
	std::string get_PrefabName();
	bool get_RenderStatus();
	void add_Child(Game_Object* po_Child_In);
	void set_Parent(Game_Object* po_Parent_In);
	Game_Object* get_Parent();
	std::map<std::string, Component*>& get_Components();
	Component* get_Component(std::string s_Name_In);
	std::map<std::string, Game_Object*>& get_Children();
	void set_Tag(std::string s_Tag_In);
	std::string get_Tag();
	bool is_Container();
	std::string get_ObjectType();

	void set_ObjectID(int i_ID_In);
	int get_ObjectID();
};