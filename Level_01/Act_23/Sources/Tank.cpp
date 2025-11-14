#include "Tank.h"

#include <GLFW/glfw3.h>

#include "Input.h"

Tank::Tank(
	const glm::vec3& position_,
	const glm::vec3& rotation_,
	const glm::vec3& scale_
) noexcept
	: Object(position_, rotation_, scale_)
	, base(std::make_unique<Object>())
	, middle(std::make_unique<Object>())
	, leftTop(std::make_unique<Object>())
	, leftBarrel(std::make_unique<Object>())
	, leftPole(std::make_unique<Object>())
	, rightTop(std::make_unique<Object>())
	, rightBarrel(std::make_unique<Object>())
	, rightPole(std::make_unique<Object>())
	, temp(std::make_unique<Object>())
	, left(std::make_unique<Object>())
	, right(std::make_unique<Object>())
	, leftBarrelPivot(std::make_unique<Object>()) 
	, leftPolePivot(std::make_unique<Object>())   
	, rightBarrelPivot(std::make_unique<Object>()) 
	, rightPolePivot(std::make_unique<Object>())
{
	base->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	base->SetScale(glm::vec3(10.0f, 2.5f, 2.5f));
	base->SetMesh(Mesh::LoadFrom("Resources/Models/Base.obj"));
	base->SetParent(this);

	temp->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	temp->SetParent(this);

	middle->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	middle->SetScale(glm::vec3(7.5f, 1.0f, 1.0f));
	middle->SetMesh(Mesh::LoadFrom("Resources/Models/Middle.obj"));
	middle->SetParent(this);

	left->SetPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
	left->SetParent(temp.get());

	leftTop->SetPosition(glm::vec3(0, 3.0f, 0.0f));
	leftTop->SetScale(glm::vec3(3.5f, 1.5f, 1.5f));
	leftTop->SetMesh(Mesh::LoadFrom("Resources/Models/Top.obj"));
	leftTop->SetParent(left.get());

	leftBarrelPivot->SetPosition(glm::vec3(0.0f, 3.0f, 0.75f));
	leftBarrelPivot->SetParent(left.get()); // 피벗의 부모 = left

	leftBarrel->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
	leftBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	leftBarrel->SetMesh(Mesh::LoadFrom("Resources/Models/Barrel.obj"));
	leftBarrel->SetParent(leftBarrelPivot.get()); // Barrel의 부모 = 피벗

	leftPolePivot->SetPosition(glm::vec3(0.0f, 3.75f, 0.0f));
	leftPolePivot->SetParent(left.get()); // 피벗의 부모 = left

	leftPole->SetPosition(glm::vec3(0.0f, 0.75f, 0.0f));
	leftPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	leftPole->SetMesh(Mesh::LoadFrom("Resources/Models/Pole.obj"));
	leftPole->SetParent(leftPolePivot.get());

	right->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
	right->SetParent(temp.get());

	rightTop->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	rightTop->SetScale(glm::vec3(3.5f, 1.5f, 1.5f));
	rightTop->SetMesh(Mesh::LoadFrom("Resources/Models/Top.obj"));
	rightTop->SetParent(right.get());

	rightBarrel->SetPosition(glm::vec3(0.0f, 3.0f, 1.0f));
	rightBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	rightBarrel->SetMesh(Mesh::LoadFrom("Resources/Models/Barrel.obj"));
	rightBarrel->SetParent(right.get());

	rightPole->SetPosition(glm::vec3(0.0f, 4.5f, 0.0f));
	rightPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	rightPole->SetMesh(Mesh::LoadFrom("Resources/Models/Pole.obj"));
	rightPole->SetParent(right.get());

	rightBarrelPivot->SetPosition(glm::vec3(0.0f, 3.0f, 0.75f));
	rightBarrelPivot->SetParent(right.get());
	rightBarrel->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
	rightBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	rightBarrel->SetMesh(Mesh::LoadFrom("Resources/Models/Barrel.obj"));
	rightBarrel->SetParent(rightBarrelPivot.get());

	// --- [수정] Right Pole ---
	rightPolePivot->SetPosition(glm::vec3(0.0f, 3.75f, 0.0f));
	rightPolePivot->SetParent(right.get());
	rightPole->SetPosition(glm::vec3(0.0f, 0.75f, 0.0f));
	rightPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	rightPole->SetMesh(Mesh::LoadFrom("Resources/Models/Pole.obj"));
	rightPole->SetParent(rightPolePivot.get());
}

Tank::~Tank() noexcept
{
	base.reset();
	middle.reset();

	leftTop.reset();
	leftBarrel.reset();
	leftBarrelPivot.reset(); 
	leftPole.reset();
	leftPolePivot.reset();   

	rightTop.reset();
	rightBarrel.reset();
	rightBarrelPivot.reset();
	rightPole.reset();
	rightPolePivot.reset();  
}

void Tank::Update(const float deltaTime_) noexcept
{
	const float horizontal = Input::IsKeyPressed(GLFW_KEY_LEFT) ?  1.0f :
							 Input::IsKeyPressed(GLFW_KEY_RIGHT) ? -1.0f : 0.0f;
	const float vertical   = Input::IsKeyPressed(GLFW_KEY_UP) ?  1.0f :
						     Input::IsKeyPressed(GLFW_KEY_DOWN) ? -1.0f : 0.0f;

	const glm::vec3 position = GetPosition();
	SetPosition(position + glm::vec3(horizontal, 0.0f, vertical) * MOVE_SPEED * deltaTime_);

	if (Input::IsKeyPressed(GLFW_KEY_T))
	{
		isAnimT = !isAnimT;
	}

	if (Input::IsKeyPressed(GLFW_KEY_L) && !isAnimL)
	{
		isAnimL = true;

		leftTargetPos = right->GetPosition();
		rightTargetPos = left->GetPosition();
	}

	if (Input::IsKeyPressed(GLFW_KEY_G))
	{
		isAnimG = true;
	}

	if (Input::IsKeyPressed(GLFW_KEY_P))
	{
		isAnimP = true;
	}

	if (isAnimT)
	{
		middle->SetRotation(middle->GetRotation() + glm::vec3(0.0f, 1.0f, 0.0f) * (10.0f * deltaTime_));
		temp->SetRotation(middle->GetRotation() + glm::vec3(0.0f, 1.0f, 0.0f) * (10.0f * deltaTime_));
	}

	if (isAnimL)
	{
		glm::vec3 leftCurrentPos  = left->GetPosition();
		glm::vec3 rightCurrentPos = right->GetPosition();

		float leftDist  = glm::distance(leftCurrentPos, leftTargetPos);
		float rightDist = glm::distance(rightCurrentPos, rightTargetPos);

		float moveAmount = MOVE_SPEED * deltaTime_;

		bool leftArrived  = false;
		bool rightArrived = false;

		if (leftDist <= moveAmount)
		{
			left->SetPosition(leftTargetPos);
			leftArrived = true;
		}
		else
		{
			glm::vec3 leftDir = glm::normalize(leftTargetPos - leftCurrentPos);
			left->SetPosition(leftCurrentPos + leftDir * moveAmount);
		}

		if (rightDist <= moveAmount)
		{
			right->SetPosition(rightTargetPos);
			rightArrived = true;
		}
		else
		{
			glm::vec3 rightDir = glm::normalize(rightTargetPos - rightCurrentPos);
			right->SetPosition(rightCurrentPos + rightDir * moveAmount);
		}

		if (leftArrived && rightArrived)
		{
			isAnimL = false;
		}
	}

	if (isAnimG)
	{
		animGTime += deltaTime_;

		const float amplitude = 45.0f;
		const float frequency = 2.0f;
		const float angle = glm::sin(animGTime * frequency) * amplitude;

		glm::vec3 leftRot = leftBarrelPivot->GetRotation();
		glm::vec3 rightRot = rightBarrelPivot->GetRotation();

		leftBarrelPivot->SetRotation(glm::vec3(leftRot.x, angle, leftRot.z));
		rightBarrelPivot->SetRotation(glm::vec3(rightRot.x, -angle, rightRot.z));
	}

	if (isAnimP)
	{
		animPTime += deltaTime_;

		const float amplitude = 45.0f;
		const float frequency = 2.0f;
		const float angle = glm::sin(animPTime * frequency) * amplitude;

		// [수정] Pole이 아닌 Pivot을 회전시킵니다.
		glm::vec3 leftRot = leftPolePivot->GetRotation();
		glm::vec3 rightRot = rightPolePivot->GetRotation();

		leftPolePivot->SetRotation(glm::vec3(angle, leftRot.y, leftRot.z));
		rightPolePivot->SetRotation(glm::vec3(-angle, rightRot.y, rightRot.z));
	}
}

void Tank::Render(
	const Shader& shader_,
	const GLenum  renderMode_
) const noexcept
{
	base->Render(shader_, renderMode_);

	middle->Render(shader_, renderMode_);

	leftTop->Render(shader_, renderMode_);
	leftBarrel->Render(shader_, renderMode_);
	leftPole->Render(shader_, renderMode_);

	rightTop->Render(shader_, renderMode_);
	rightBarrel->Render(shader_, renderMode_);
	rightPole->Render(shader_, renderMode_);
}