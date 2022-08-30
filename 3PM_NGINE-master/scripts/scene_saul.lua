-- Mi test lua
rotation = 0.0
timer = 0.0

function start()

  sand_albedo = LoadTexture("../../data/textures/sand/SandTest2_basecolor.jpg")
  sand_normal = LoadTexture("../../data/textures/sand/SandTest2_normal.jpg")
  sand_spec = LoadTexture("../../data/textures/sand/SandTest2_ambientocclusion.jpg")

  rock_albedo = LoadTexture("../../data/textures/Rock/rock_albedo.jpg")
  rock_ao = LoadTexture("../../data/textures/Rock/rock_ao.jpg")
  rock_height = LoadTexture("../../data/textures/Rock/rock_height.jpg")
  rock_metallic = LoadTexture("../../data/textures/Rock/rock_metallic.jpg")
  rock_normal = LoadTexture("../../data/textures/Rock/rock_normal.jpg")
  rock_roughness = LoadTexture("../../data/textures/Rock/rock_roughness.jpg")

--LIGHTS
  light_shadow_1  = CreateEntity()
  AddMaterialComponent(light_shadow_1)
  AddTransformComponent(light_shadow_1)
  SetPosition(light_shadow_1, 20.0, 50.0, 20.0)
  SetRotation(light_shadow_1, 80.0, -50.0, 0.0)
  SetScale(light_shadow_1, 0.5, 0.5, 0.5)
  AddLightComponent(light_shadow_1, 2)
  SetLightColor(light_shadow_1, 0.2, 0.2, 0.2)
  SetGeometry(light_shadow_1, 1)
  
  light_shadow_2  = CreateEntity()
  AddMaterialComponent(light_shadow_2)
  AddTransformComponent(light_shadow_2)
  SetPosition(light_shadow_2, 20.0, 50.0, -20.0)
  SetRotation(light_shadow_2, 145.0, 0.0, 0.0)
  SetScale(light_shadow_2, 0.5, 0.5, 0.5)
  AddLightComponent(light_shadow_2, 2)
  SetLightColor(light_shadow_2, 0.0, 0.2, 0.0)
  SetGeometry(light_shadow_2, 1)
  
  light_shadow_3  = CreateEntity()
  AddMaterialComponent(light_shadow_3)
  AddTransformComponent(light_shadow_3)
  SetPosition(light_shadow_3, -20.0, 50.0, 20.0)
  SetRotation(light_shadow_3, 100.0, 55.0, 0.0)
  SetScale(light_shadow_3, 0.5, 0.5, 0.5)
  AddLightComponent(light_shadow_3, 2)
  SetLightColor(light_shadow_3, 0.0, 0.0, 0.2)
  SetGeometry(light_shadow_3, 1)
  
  light_shadow_4  = CreateEntity()
  AddMaterialComponent(light_shadow_4)
  AddTransformComponent(light_shadow_4)
  SetPosition(light_shadow_4, -20.0, 50.0, -20.0)
  SetRotation(light_shadow_4, 35.0, 15.0, 0.0)
  SetScale(light_shadow_4, 0.5, 0.5, 0.5)
  AddLightComponent(light_shadow_4, 2)
  SetLightColor(light_shadow_4, 0.2, 0.0, 0.2)
  SetGeometry(light_shadow_4, 1)

  light_point  = CreateEntity()
  AddMaterialComponent(light_point)
  AddTransformComponent(light_point)
  SetPosition(light_point, 0.0, 40.0, -180.0)
  SetRotation(light_point, 35.0, 15.0, 0.0)
  SetScale(light_point, 0.5, 0.5, 0.5)
  AddLightComponent(light_point, 4)
  SetLightColor(light_point, 0.8, 0.8, 0.8)
  SetLightDiffuseStrength(light_point, 1)
  SetLightSpecularStrength(light_point, 2)
  SetGeometry(light_point, 1)

  light_spot  = CreateEntity()
  AddMaterialComponent(light_spot)
  AddTransformComponent(light_spot)
  SetPosition(light_spot, -40.0, -25.0, -20.0)
  SetRotation(light_spot, -90, 0.0, 0.0)
  SetScale(light_spot, 0.5, 0.5, 0.5)
  AddLightComponent(light_spot, 3)
  SetLightColor(light_spot, 0.0, 0.8, 0.8)
  SetLightDiffuseStrength(light_spot, 8)
  SetLightSpecularStrength(light_spot, 2)
  SetLightConeRadius(light_spot, 1)
  SetGeometry(light_spot, 1)

--SPHERE
  sphere_  = CreateEntity()
  AddMaterialComponent(sphere_)
  AddTransformComponent(sphere_)
  SetGeometry(sphere_, 1)
  SetPosition(sphere_, 0.0, 0.0, 0.0)
  SetScale(sphere_, 15.0, 15.0, 15.0)
  AddAnimatorComponent(sphere_, 5)
  SetTextureAtIndex(sphere_, rock_albedo,0)
  SetTextureAtIndex(sphere_, rock_normal,1)
  SetTextureAtIndex(sphere_, rock_metallic,2)
  SetTextureAtIndex(sphere_, rock_ao,4)
  SetTextureAtIndex(sphere_, rock_metallic,5)
  SetTextureAtIndex(sphere_, rock_roughness,6)

--FLOOR
  floor_  = CreateEntity()
  AddMaterialComponent(floor_)
  AddTransformComponent(floor_)
  SetGeometry(floor_, 6)
  SetPosition(floor_, 0.0 , -30.0, 0.0)
  SetScale(floor_, 120.0, 1.0, 120.0)
  SetRotation(floor_, 0.0, 0.0, 0.0)
  SetTextureAtIndex(floor_, sand_albedo, 0)
  SetTextureAtIndex(floor_, sand_normal, 1)
  SetTextureAtIndex(floor_, sand_spec, 2)

  --DEMO MATERIAL
  PlaneDemo  = CreateEntity()
  AddMaterialComponent(PlaneDemo)
  AddTransformComponent(PlaneDemo)
  SetGeometry(PlaneDemo, 6)
  SetPosition(PlaneDemo, 0.0 , 40.0, -200.0)
  SetScale(PlaneDemo, 60.0, 1.0, 60.0)
  SetRotation(PlaneDemo, 90.0, 0.0, 0.0)
  SetTextureAtIndex(PlaneDemo, rock_albedo, 0)
  SetTextureAtIndex(PlaneDemo, rock_normal,1)
  SetTextureAtIndex(PlaneDemo, rock_metallic,2)
  SetTextureAtIndex(PlaneDemo, rock_ao,4)
  SetTextureAtIndex(PlaneDemo, rock_metallic,5)
  SetTextureAtIndex(PlaneDemo, rock_roughness,6)

end

function update()
  
  timer = timer + deltaTime
  --SetPosition(sphere_, 0.0 + math.cos(timer) * 25.0, 15, 0.0)
  --SetRotation(sphere_, 0.0, 0.0, timer * 10.0)

end