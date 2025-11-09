#include <iostream>

#include "Application.h"

/**
 * @brief 
 */
static void OnStart() noexcept;

/**
 * @brief 
 */
static void OnUpdate() noexcept;

/**
 * @brief 
 */
static void OnRender() noexcept;

/**
 * @brief 
 */
static void OnQuit() noexcept;

int main(int argc_, char** argv_)
{
	Application::Config config	= {};
	config.argc					= argc_;
	config.argv					= argv_;
	config.fps					= 60;
	config.onStart				= OnStart;
	config.onUpdate				= OnUpdate;
	config.onRender			    = OnRender;
	config.onQuit				= OnQuit;
	
	return Application::Run(config);
}

void OnStart() noexcept
{
	std::cout << "Application Loaded!" << std::endl;
}

void OnUpdate() noexcept
{
	std::cout << "Application Idle..." << std::endl;
}

void OnRender() noexcept
{
	std::cout << "Application Displaying Frame!" << std::endl;
}

void OnQuit() noexcept
{
	std::cout << "Application Closing!" << std::endl;
}