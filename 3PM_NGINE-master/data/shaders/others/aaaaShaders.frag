#version 460
uniform layout ( binding = 0 ) sampler2D albedo;
uniform layout ( binding = 1 ) sampler2D normal;
uniform layout ( binding = 2 ) sampler2D metal;
uniform layout ( binding = 3 ) sampler2D specular;

uniform layout ( binding = 4 ) sampler2D shadowMap1 ;
uniform layout ( binding = 5 ) sampler2D shadowMap2 ;
uniform layout ( binding = 6 ) sampler2D shadowMap3 ;
uniform layout ( binding = 7 ) sampler2D shadowMap4 ;
uniform layout ( binding = 8 ) sampler2D shadowMap5 ;
uniform layout ( binding = 9 ) sampler2D shadowMap6 ;
uniform layout ( binding = 10) sampler2D shadowMap7 ;
uniform layout ( binding = 11) sampler2D shadowMap8 ;
uniform layout ( binding = 12) sampler2D shadowMap9 ;
uniform layout ( binding = 13) sampler2D shadowMap10;
uniform layout ( binding = 14) sampler2D shadowMap11;
uniform layout ( binding = 15) sampler2D shadowMap12;
uniform layout ( binding = 16) sampler2D shadowMap13;
uniform layout ( binding = 17) sampler2D shadowMap14;
uniform layout ( binding = 18) sampler2D shadowMap15;
uniform layout ( binding = 19) sampler2D shadowMap16;



layout(std140, binding = 1) uniform Material
{
    uniform vec4 albedoColor;
    uniform vec4 metalic; //x:% , y:smoothness

    uniform ivec4 flags; //x: albedo texture, y: normal map texture, z: metalic texture, w: specular texture

};

in vec2 uv_;
in vec3 FragPos;
in vec3 Normal;
in vec3 viewPos;
out vec4 FragColor;

vec3 diffuseG = vec3(0.0);
vec3 ambientG = vec3(0.0);
vec3 specularG = vec3(0.0);
vec3 shadowColorG= vec3(0.0);
float shadowG=1.0f;

bool direcctionalDone = false;


struct Light{
    ivec4 Type;//x={0:directional, 1:spotlight, 2: pointlight} y=castShadow
    vec4 Pos;//w = trash
    vec4 Dir;//w = trash
    vec4 ColorDiffuse;//xyz:color  ,w: opacity
    vec4 Ambient; //xyz:color  ,w: strength
    vec4 SpecularColor;//xyz:color  ,w: strength
    vec4 values;//x: shyniness, y: linear,  z:quadratic
    vec4 values2;//x: inner, y: outter

    vec4 shadowColor;

    vec4 space;
    mat4 v;
    mat4 p;
};

layout(std140, binding = 3) uniform Lightning
{
        Light lights[16];
};

void DirectionalLight(int index){

    Light light = lights[index];
    

    vec3 ambientL = light.Ambient.xyz * light.Ambient.w * albedoColor.xyz;

    
    vec3 specularL = vec3(0.0,0.0,0.0);

    vec3 normalN = normalize(Normal);

    vec3 lightDirNormal=normalize(light.Dir.xyz);
    vec3 difusseL = light.ColorDiffuse.xyz * light.ColorDiffuse.w *
                    max(dot(normalN, -lightDirNormal), 0.0);
                    //* albedoL;         

    specularL = light.SpecularColor.xyz * light.SpecularColor.w *
                    pow(max(dot(normalize(viewPos - FragPos), 
                    reflect(lightDirNormal, normalN))
			        , 0.0), light.values.x) ;

	//Adds the light value to de global ilumination
	diffuseG += difusseL;
	ambientG = max(ambientG,ambientL);
	specularG += specularL;

}
void SpotLight(int index){
    Light light = lights[index];
    
    vec3 lightDir=normalize(light.Pos.xyz-FragPos);
    vec3 normal=normalize(Normal);
    vec3 viewDir= normalize(viewPos-FragPos);
    float distance= length(light.Pos.xyz-FragPos);

    float theta = dot(lightDir, -normalize(light.Dir.xyz));
    float epsilon= light.values2.x-light.values2.y;
    float intensity = clamp((theta - light.values2.y) / epsilon, 0.0, 1.0); 
    
    float darkSide=clamp(dot(lightDir,normal), 0.0, 1.0);
    float attenuation = 1.0f / (1.0 + light.values.z * distance + light.values.w * (distance * distance));

    diffuseG+=light.ColorDiffuse.xyz*light.ColorDiffuse.w*intensity*darkSide*attenuation;
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.values.x);

    specularG+=light.SpecularColor.xyz*light.SpecularColor.w*specular*attenuation;
    
	//FragColor = vec4(vec3(theta), 1.0);

}
void PointLight(int index){
    
    Light light = lights[index];

    vec3 normal=normalize(Normal);
    vec3 viewDir= normalize(viewPos-FragPos);
    float distance=length(light.Pos.xyz-FragPos);
    vec3 lightDir = normalize(light.Pos.xyz - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float attenuation = light.values2.y / (1.0 + light.values.y * distance + 
  			     light.values.z * (distance * distance));    
    float darkSide=clamp(dot(lightDir,normal), 0.0, 1.0);
   
    diffuseG  += light.ColorDiffuse.xyz*light.ColorDiffuse.w*attenuation*darkSide;


    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.values.x);

    specularG += light.SpecularColor.xyz*light.SpecularColor.w*specular*darkSide;

}

void calculateLight(int index){
    switch(lights[index].Type.x){
        case 1:
		if(!direcctionalDone){
            DirectionalLight(index);
			direcctionalDone = true;
		}
            break;
        case 2:
            SpotLight(index);
                
            

            break;
        case 3:
            PointLight(index);
            break;
    }
    
}
vec4 GetShadowMap(int id,vec2 cord) {
  switch (id){
    case 0: return texture(shadowMap1 ,cord,1.1);
    case 1: return texture(shadowMap2 ,cord,1.1);
    case 2: return texture(shadowMap3 ,cord,1.1);
    case 3: return texture(shadowMap4 ,cord,1.1);
    case 4: return texture(shadowMap5 ,cord,1.1);
    case 5: return texture(shadowMap6 ,cord,1.1);
    case 6: return texture(shadowMap7 ,cord,1.1);
    case 7: return texture(shadowMap8 ,cord,1.1);
    case 8: return texture(shadowMap9 ,cord,1.1);
    case 9: return texture(shadowMap10,cord,1.1);
    case 10:return texture(shadowMap11,cord,1.1);
    case 11:return texture(shadowMap12,cord,1.1);
    case 12:return texture(shadowMap13,cord,1.1);
    case 13:return texture(shadowMap14,cord,1.1);
    case 14:return texture(shadowMap15,cord,1.1);
    case 15:return texture(shadowMap16,cord,1.1);

  }
  return vec4(1.0);
}
void CalculateShadows(){
    for(int i = 0; i<16&& shadowG>0.0; i++){
        if(lights[i].Type.y==1){
            if(lights[i].Type.x==1){
                vec4 pos= (lights[i].p * lights[i].v*vec4(FragPos,1.0));
                vec3 lightCoords=pos.xyz/pos.w;

                //vec3 psm=pos.xyz/pos.w;
                if(lightCoords.z<=1.0){
                    lightCoords=(lightCoords+1.0)/2.0;


                    //float closestDepth=texture(smap,lightCoords.xy).r+0.0001;


                    if(lightCoords.x>=0.0f && lightCoords.x<=1.0f && lightCoords.y>=0.0f && lightCoords.y<=1.0f){
                        float closestDepth=GetShadowMap(i, lightCoords.xy).r+0.0001;

                        if(lightCoords.z>closestDepth){
                            shadowG=0.0;
                            shadowColorG= lights[i].shadowColor.xyz;
                        }

                    }
                    //shadowG=closestDepth;
                }
                
            }else if(lights[i].Type.x==2){
                vec4 pos= (lights[i].p * lights[i].v*vec4(FragPos,1.0));
                vec3 lightCoords=pos.xyz/pos.w;

                //vec3 psm=pos.xyz/pos.w;
                if(lightCoords.z<=1.0){
                    lightCoords=(lightCoords+1.0)/2.0;

                    if(lightCoords.x>=0.0f && lightCoords.x<=1.0f && lightCoords.y>=0.0f && lightCoords.y<=1.0f){
                      float closestDepth=GetShadowMap(i, lightCoords.xy).r+0.0001;
                        if(lightCoords.z>closestDepth){
                            shadowG=0.0;
                            shadowColorG = lights[i].shadowColor.xyz;
                        }  
                    }
                    
                    
                }
                
            }
        }
    }
    
}
void main(){

    vec4 lightValue = vec4(0.0);

    for(int i = 0; i<16; i++){
        calculateLight(i);
    }

	vec3 albedoL = albedoColor.xyz;
	if (flags.x!=0){ //flags.x == 1 if albedo enabled
        albedoL = albedoColor.xyz * texture(albedo, uv_).rgb;
    }
	if (flags.w!=0){ //flags.w == 1 if specular enabled
        specularG *= texture(specular, uv_).rgb;
    }
	CalculateShadows();
	FragColor = vec4((ambientG+(diffuseG*(shadowG))+specularG)*albedoL+((1.0-shadowG)* shadowColorG.xyz), 1.0);
	//FragColor = vec4((diffuseG)*albedoL, 1.0);
	//FragColor = vec4((specularG)*albedoL, 1.0);
	//FragColor = vec4(texture(shadowMap1, uv_.yx).rgb, 1.0);
    //FragColor = vec4(vec3(shadowG), 1.0);

}