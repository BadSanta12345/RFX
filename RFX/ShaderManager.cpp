#include "Refractor.h"
#include "ShaderHook.h"

D3DXCreateEffect_t pOrigD3DXCreateEffect = D3DXCreateEffect;

HRESULT WINAPI MyD3DXCreateEffect(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen, CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT* ppEffect, LPD3DXBUFFER* ppCompilationErrors)
{
	auto str = reinterpret_cast<const char*>(pSrcData);

	const char* begin = nullptr;
	const char* end = nullptr;

	while (*str && *str != '\n')
	{
		if (*str == '"')
		{
			if (begin)
			{
				end = str;
				break;
			}
			else
			{
				begin = ++str;
			}
		}
		++str;
	}
	std::string name;
	if (begin && end)
	{
		while (begin != end)
		{
			name.push_back(*begin);
			++begin;
		}
	}

	auto retVal = pOrigD3DXCreateEffect(pDevice, pSrcData, SrcDataLen, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);
	if (name != "")
	{
		DICE::detail::gShaders[name] = *ppEffect;
	}
	/*std::fstream file;
	file.open("RFX\\shaders.txt", std::fstream::out | std::fstream::app);
	file << name << ": " << *ppEffect << " Defines: {";
	while (pDefines && pDefines->Name)
	{
		file << pDefines->Name << '=' << pDefines->Definition << ';';
		++pDefines;
	}
	file << "}\n";
	file << str;
	file.close();*/
	return retVal;
}

namespace DICE
{
	namespace detail
	{
		::std::map<::std::string, ID3DXEffect*> gShaders;
	}
	namespace ShaderManager
	{
		ID3DXEffect* getShaderByName(const ::std::string& name)
		{
			auto it = detail::gShaders.find(name);
			if (it != detail::gShaders.end())
			{
				return it->second;
			}
			return nullptr;
		}

		void init()
		{
			createConsoleObject(&setFloatBySemantic, "shadermanagersetFloatBySemantic");
			createConsoleObject(&setFloat2BySemantic, "shadermanagersetFloat2BySemantic");
			createConsoleObject(&setFloat3BySemantic, "shadermanagersetFloat3BySemantic");
			createConsoleObject(&setFloat4BySemantic, "shadermanagersetFloat4BySemantic");

			// TODO: stream operator from string to IDirect3DTexture9* 
			//createConsoleObject(&setShaderTextureBySemantic, "shadermanagersetShaderTextureBySemantic");
		}

		::std::string setFloatBySemantic(const ::std::string& shaderName, const ::std::string& semantic, float val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetFloat(hParameter, val);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}

		::std::string setFloat2BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR2& val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetFloatArray(hParameter, reinterpret_cast<const float*>(&val), 2);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}

		::std::string setFloat3BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR3& val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetFloatArray(hParameter, reinterpret_cast<const float*>(&val), 3);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}

		::std::string setFloat4BySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXVECTOR4& val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetFloatArray(hParameter, reinterpret_cast<const float*>(&val), 4);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}

		::std::string setMatrixBySemantic(const ::std::string& shaderName, const ::std::string& semantic, const D3DXMATRIX& val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetMatrix(hParameter, &val);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}

		::std::string setTextureBySemantic(const ::std::string& shaderName, const ::std::string& semantic, IDirect3DTexture9* val)
		{
			auto pShader = getShaderByName(shaderName);
			if (!pShader) return "Error: Shader not found.";
			auto hParameter = pShader->GetParameterBySemantic(NULL, semantic.c_str());
			if (!hParameter) return "Error: Parameter not found.";
			auto hr = pShader->SetTexture(hParameter, val);
			if (hr != S_OK) return "Error: Failed to set parameter value.";
			return "1";
		}
	}
}