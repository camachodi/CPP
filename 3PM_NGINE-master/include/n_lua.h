#ifndef __N_LUA_H__
#define __N_LUA_H__ 1

#include "lua.hpp"
#include "n_window.h"
#include "n_ecs.h"
#include "n_gpumanager.h"
#include "n_dlist.h"
#include "n_render.h"

namespace NGINE
{

	class Lua
	{

	public:

		Lua();
		~Lua();

		int LoadLuaFile(char* route);

		int Initialize();

		int isReady();

		static int CreateObject(lua_State* LS);

		static int SetPosition(lua_State* LS);

		static int SetRotation(lua_State* LS);

		static int SetObjScale(lua_State* LS);
		
		static int SetMesh(lua_State* LS);
		
		static int SetMaterial(lua_State* LS);
		
		static int SetScale(lua_State* LS);
		
		static int SetParent(lua_State* LS);
		
		static int RemoveParent(lua_State* LS);
		
		static int SetColor(lua_State* LS);
		
		static int CreateLight(lua_State* LS);
		
		static int CreatePostprocess(lua_State* LS);
		
		static int SetShadowColor(lua_State* LS);

		static int SetPProccess(lua_State* LS, NGINE::PProcess ppr);
		
		void Execute(char* exec);
		
		void DrawLuaObj(NGINE::DList* dlist);

	protected:

		bool init_;
		bool loaded_;
		lua_State* LS_;

	private:

		Lua(const Lua& c){}

	};

}

#endif
