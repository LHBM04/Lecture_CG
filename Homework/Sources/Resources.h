#pragma once

#include "PCH.h"

class Mesh;

/**
 * @brief 애플리케이션 내 사용될 리소스를 관리합니다.
 */
class Resources final
{
public:
	/**
	 * @brief 지정한 경로로부터 불러온 셰이더를 가져옵니다.
	 * 
	 * @param path_ 지정할 경로
	 * 
	 * @return const std::string& 불러온 셰이더의 소스 코드
	 */
	[[nodiscard]]
	static const std::string& GetShader(std::string_view path_) noexcept;

	/**
	 * @brief 지정한 경로로부터 불러온 메쉬를 가져옵니다.
	 * 
	 * @param path_ 지정할 경로
	 * 
	 * @return Mesh* const 불러온 메쉬.
	 */
	[[nodiscard]]
	static Mesh* const GetMesh(std::string_view path_) noexcept;

private:
	/**
	 * @brief 지정한 경로로부터 셰이더를 불러옵니다.
	 * 
	 * @param path_ 지정할 경로
	 */
	static void LoadShader(std::string_view path_) noexcept;

	/**
	 * @brief 지정한 경로로로부터 메쉬를 불러옵니다.
	 * 
	 * @param path_ 지정할 경로
	 */
	static void LoadMesh(std::string_view path_) noexcept;

	/**
	 * @brief 불러온 셰이더들.
	 */
	static std::unordered_map<std::filesystem::path, std::string> shaders;

	/**
	 * @brief 불러온 메쉬들.
	 */
	static std::unordered_map<std::filesystem::path, std::unique_ptr<Mesh>> meshes;
};