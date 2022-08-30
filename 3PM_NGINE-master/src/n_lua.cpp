#include "..\include\n_lua.h"

NGINE::Lua::Lua()
{

	LS_ = luaL_newstate();
	luaL_openlibs(LS_);
	init_ = false;
	loaded_ = false;

}

NGINE::Lua::~Lua()
{

	if (nullptr != LS_)
	{

		lua_close(LS_);

	}

}

int NGINE::Lua::LoadLuaFile(char* path)
{

	int load_ = luaL_dofile(LS_, path);

	if (load_ != LUA_OK)
	{

		const char* path_ = lua_tostring(LS_, -1);
		fprintf(stderr, "ERROR: cannot find %s\n", path_);

	}
	else
	{

		loaded_ = true;

	}

	return load_;

}

int NGINE::Lua::Initialize()
{

	init_ = true;

	lua_pushcfunction(LS_, CreateObject);
	lua_setglobal(LS_, "CreateObject");

	lua_pushcfunction(LS_, SetPosition);
	lua_setglobal(LS_, "SetPosition");

	lua_pushcfunction(LS_, SetRotation);
	lua_setglobal(LS_, "SetRotation");

	lua_pushcfunction(LS_, SetObjScale);
	lua_setglobal(LS_, "SetObjScale");

	lua_pushcfunction(LS_, SetScale);
	lua_setglobal(LS_, "SetScale");
	
	lua_pushcfunction(LS_, SetMesh);
	lua_setglobal(LS_, "SetMesh");
	
	lua_pushcfunction(LS_, SetMaterial);
	lua_setglobal(LS_, "SetMaterial");
	
	lua_pushcfunction(LS_, SetParent);
	lua_setglobal(LS_, "SetParent");
	
	lua_pushcfunction(LS_, RemoveParent);
	lua_setglobal(LS_, "RemoveParent");
	
	lua_pushcfunction(LS_, SetColor);
	lua_setglobal(LS_, "SetColor");
	
	lua_pushcfunction(LS_, CreateLight);
	lua_setglobal(LS_, "CreateLightDirectional");
	
	lua_pushcfunction(LS_, CreatePostprocess);
	lua_setglobal(LS_, "CreatePostprocess");
	
	lua_pushcfunction(LS_, SetShadowColor);
	lua_setglobal(LS_, "SetShadowColor");


	return init_;

}

int NGINE::Lua::isReady()
{

	return (init_ && loaded_);

}

int NGINE::Lua::CreateObject(lua_State* LS)
{

	int args = lua_gettop(LS);

	NGINE::GPUManager* gpu = NGINE::GPUManager::GPUData();
	NGINE::Object obj = gpu->CreateObject();
	NGINE::Material material = NGINE::Material(0);
	NGINE::Mesh mesh = NGINE::Mesh(0);
	obj.SetMaterial(material);
	obj.SetMesh(mesh);

	NGINE::GPUManager::GPUData()->ObjLua.push_back(obj);
	lua_pushinteger(LS, obj.GetID());

	if (args != 0)
	{

		return luaL_error(LS, "No arguments needed");
	
	}

	return 1;

}

int NGINE::Lua::SetPosition(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (luaL_checknumber(LS, 2));
		float y = (luaL_checknumber(LS, 3));
		float z = (luaL_checknumber(LS, 4));

		NGINE::Transform(NGINE::Object(index).GetTransform()).SetPosition(glm::vec3(x, y, z));
	
	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");

	}

	return LUA_OK;

}

int NGINE::Lua::SetRotation(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (luaL_checknumber(LS, 2));
		float y = (luaL_checknumber(LS, 3));
		float z = (luaL_checknumber(LS, 4));

		NGINE::Transform(NGINE::Object(index).GetTransform()).SetRotation(glm::vec3(x, y, z));
	
	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");

	}

	return LUA_OK;

}

int NGINE::Lua::SetObjScale(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (luaL_checknumber(LS, 2));
		float y = (luaL_checknumber(LS, 3));
		float z = (luaL_checknumber(LS, 4));

		NGINE::Transform(NGINE::Object(index).GetTransform()).SetScale(glm::vec3(x, y, z));

	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");

	}

	return LUA_OK;

}

int NGINE::Lua::SetMesh(lua_State* LS)
{

	int args = lua_gettop(LS);
	
	if (args != 2)
	{

		return luaL_error(LS, "Need 2 arguments");

	}

	size_t* len = nullptr;
	int index = (int)(luaL_checkinteger(LS, 1));

	std::string name = luaL_checklstring(LS, 2, len);
	NGINE::Mesh m = NGINE::GPUManager::GPUData()->LoadMesh(name.c_str());
	NGINE::Object(index).SetMesh(m);

	return 0;

}

int NGINE::Lua::SetMaterial(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args != 4)
	{

		return luaL_error(LS, "Need 4 arguments");

	}

	int index = (int)(luaL_checkinteger(LS, 1));

	size_t* len = nullptr;
	std::string shaderFrag = luaL_checklstring(LS, 2, len);
	std::string shaderVert = luaL_checklstring(LS, 2, len);
	std::string textureRoute = luaL_checklstring(LS, 3, len);

	NGINE::Shader shader = NGINE::GPUManager::GPUData()->LoadShader(shaderFrag.c_str(), shaderVert.c_str());
	NGINE::Texture tex = NGINE::GPUManager::GPUData()->LoadTexture(textureRoute.c_str());
	NGINE::Material mat = NGINE::GPUManager::GPUData()->CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, shader);
	
	mat.LoadShader(shader);
	mat.LoadTextureAlbedo(tex);
	NGINE::Object(index).SetMaterial(mat);

	return 0;

}

int NGINE::Lua::SetScale(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (luaL_checknumber(LS, 2));
		float y = (luaL_checknumber(LS, 3));
		float z = (luaL_checknumber(LS, 4));

		glm::vec3 dsScale = NGINE::Transform(NGINE::Object(index).GetTransform()).GetScale();
		NGINE::Transform(NGINE::Object(index).GetTransform()).SetScale(dsScale);
	
	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");
	
	}

	return LUA_OK;

}

int NGINE::Lua::SetParent(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args != 2)
	{

		return luaL_error(LS, "Need 2 arguments");

	}

	int parent = (int)(luaL_checkinteger(LS, 1));
	int child = (int)(luaL_checkinteger(LS, 2));

	NGINE::Object(child).GetTransform().SetParent(NGINE::Object(parent).GetTransform());

	return 0;

}

int NGINE::Lua::RemoveParent(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args != 1)
	{

		return luaL_error(LS, "Need just 1 argument");

	}

	int obj = (int)(luaL_checkinteger(LS, 1));
	NGINE::Transform tr_ = NGINE::Transform(0);
	NGINE::Object(obj).GetTransform().SetParent(tr_);

	return 0;

}

int NGINE::Lua::SetColor(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (float)(luaL_checknumber(LS, 2));
		float y = (float)(luaL_checknumber(LS, 3));
		float z = (float)(luaL_checknumber(LS, 4));

		NGINE::Material(NGINE::Object(index).GetMaterial()).SetColor(x, y, z, 1.0f);
	
	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");
	
	}

	return LUA_OK;

}

int NGINE::Lua::CreateLight(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 1)
	{
	
		NGINE::GPUManager* gpu = NGINE::GPUManager::GPUData();
		int Itype_ = (int)(luaL_checkinteger(LS, 1));
		
		LightType type_;

		if (Itype_ == 0)
		{
			
			type_ = NONEL;

		}
		else if (Itype_ == 1)
		{

			type_ = DIRECTIONAL;

		}
		else if (Itype_ == 2)
		{

			type_ = SPOT;

		}
		else if (Itype_ == 3)
		{

			type_ = POINT;

		}

		NGINE::Light l_ = gpu->CreateLight(type_);

		NGINE::GPUManager::GPUData()->ObjLua.push_back(l_.GetObject());
		lua_pushinteger(LS, l_.GetId());

	}
	else
	{

		return luaL_error(LS, "Just need 1 argument");
	
	}

	return 1;

}

int NGINE::Lua::CreatePostprocess(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args != 2)
	{

		return luaL_error(LS, "Just need 1 argument");

	}

	size_t* len = nullptr;

	std::string shaderFrag = luaL_checklstring(LS, 1, len);
	std::string shaderVert = luaL_checklstring(LS, 1, len);

	NGINE::Shader shader = NGINE::GPUManager::GPUData()->LoadShader(shaderFrag.c_str(), shaderVert.c_str());
	NGINE::GPUManager* gpu = NGINE::GPUManager::GPUData();
	gpu->ApplyPPScreenShader(shader);

	return 0;

}

int NGINE::Lua::SetShadowColor(lua_State* LS)
{

	int args = lua_gettop(LS);

	if (args == 4)
	{

		int index = (int)(luaL_checkinteger(LS, 1));
		float x = (float)(luaL_checknumber(LS, 2));
		float y = (float)(luaL_checknumber(LS, 3));
		float z = (float)(luaL_checknumber(LS, 4));

		NGINE::Object(index).GetLight().SetColorShadow(glm::vec4(x, y, z, 1.0f));

	}
	else
	{

		return luaL_error(LS, "Need 4 arguments");

	}

	return LUA_OK;

}


int NGINE::Lua::SetPProccess(lua_State* LS, NGINE::PProcess ppr)
{

	int args = lua_gettop(LS);

	if (args != 2)
	{

		return luaL_error(LS, "Need 2 arguments");

	}

	int index = (int)(luaL_checkinteger(LS, 1));


}

void NGINE::Lua::Execute(char* exec)
{

	if (isReady())
	{

		lua_getglobal(LS_, exec);
		lua_call(LS_, 0, 0);

	}

}

void NGINE::Lua::DrawLuaObj(NGINE::DList* dlist)
{

	auto& luaObjsToDraw = NGINE::GPUManager::GPUData()->ObjLua;

	for (int i = 0; i < luaObjsToDraw.size(); i++)
	{

		dlist->DoDraw(luaObjsToDraw[i]);

	}

}