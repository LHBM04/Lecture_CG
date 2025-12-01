#include "Tank.h"

#include <GLFW/glfw3.h>

#include "Input.h"
#include "Mesh.h"

Tank::Tank() noexcept
	: Object(nullptr)
{
	barrelMesh = Mesh::LoadFrom("Resources/Meshes/Barrel.obj");
	poleMesh   = Mesh::LoadFrom("Resources/Meshes/Pole.obj");
	topMesh    = Mesh::LoadFrom("Resources/Meshes/Top.obj");
	baseMesh   = Mesh::LoadFrom("Resources/Meshes/Base.obj");
	middleMesh = Mesh::LoadFrom("Resources/Meshes/Middle.obj");

	base = std::make_unique<Object>(baseMesh.get());
	base->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	base->SetScale(glm::vec3(10.0f, 2.5f, 2.5f));
	base->SetParent(this);

	temp = std::make_unique<Object>(nullptr);
	temp->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	temp->SetParent(this);

	middle = std::make_unique<Object>(middleMesh.get());
	middle->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	middle->SetScale(glm::vec3(7.5f, 1.0f, 1.0f));
	middle->SetParent(this);

	left = std::make_unique<Object>(nullptr);
	left->SetPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
	left->SetParent(temp.get());

	leftTop = std::make_unique<Object>(topMesh.get());
	leftTop->SetPosition(glm::vec3(0, 3.0f, 0.0f));
	leftTop->SetScale(glm::vec3(3.5f, 1.5f, 1.5f));
	leftTop->SetParent(left.get());

	leftBarrel = std::make_unique<Object>(barrelMesh.get());
	leftBarrel->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
	leftBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	leftBarrel->SetParent(leftBarrelPivot.get());

	leftBarrelPivot = std::make_unique<Object>(nullptr);
	leftBarrelPivot->SetPosition(glm::vec3(0.0f, 3.0f, 0.75f));
	leftBarrelPivot->SetParent(left.get());

	leftPolePivot = std::make_unique<Object>(nullptr);
	leftPolePivot->SetPosition(glm::vec3(0.0f, 3.75f, 0.0f));
	leftPolePivot->SetParent(left.get());

	leftPole = std::make_unique<Object>(poleMesh.get());
	leftPole->SetPosition(glm::vec3(0.0f, 0.75f, 0.0f));
	leftPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	leftPole->SetParent(leftPolePivot.get());

	right = std::make_unique<Object>(nullptr);
	right->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
	right->SetParent(temp.get());

	rightTop = std::make_unique<Object>(topMesh.get());
	rightTop->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	rightTop->SetScale(glm::vec3(3.5f, 1.5f, 1.5f));
	rightTop->SetParent(right.get());

	rightBarrel = std::make_unique<Object>(barrelMesh.get());
	rightBarrel->SetPosition(glm::vec3(0.0f, 3.0f, 1.0f));
	rightBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	rightBarrel->SetParent(right.get());

	rightPole = std::make_unique<Object>(poleMesh.get());
	rightPole->SetPosition(glm::vec3(0.0f, 4.5f, 0.0f));
	rightPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	rightPole->SetParent(right.get());

	rightBarrel = std::make_unique<Object>(barrelMesh.get());
	rightBarrel->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
	rightBarrel->SetScale(glm::vec3(0.5f, 0.5f, 3.0f));
	rightBarrel->SetParent(rightBarrelPivot.get());

	rightBarrelPivot = std::make_unique<Object>(nullptr);
	rightBarrelPivot->SetPosition(glm::vec3(0.0f, 3.0f, 0.75f));
	rightBarrelPivot->SetParent(right.get());

	rightPole = std::make_unique<Object>(poleMesh.get());
	rightPole->SetPosition(glm::vec3(0.0f, 0.75f, 0.0f));
	rightPole->SetScale(glm::vec3(0.5f, 1.5f, 0.5f));
	rightPole->SetParent(rightPolePivot.get());

	rightPolePivot = std::make_unique<Object>(nullptr);
	rightPolePivot->SetPosition(glm::vec3(0.0f, 3.75f, 0.0f));
	rightPolePivot->SetParent(right.get());
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

		// [����] Pole�� �ƴ� Pivot�� ȸ����ŵ�ϴ�.
		glm::vec3 leftRot = leftPolePivot->GetRotation();
		glm::vec3 rightRot = rightPolePivot->GetRotation();

		leftPolePivot->SetRotation(glm::vec3(angle, leftRot.y, leftRot.z));
		rightPolePivot->SetRotation(glm::vec3(-angle, rightRot.y, rightRot.z));
	}
}

void Tank::Render(const Shader& shader_) const noexcept
{
	base->Render(shader_);

	middle->Render(shader_);

	leftTop->Render(shader_);
	leftBarrel->Render(shader_);
	leftPole->Render(shader_);

	rightTop->Render(shader_);
	rightBarrel->Render(shader_);
	rightPole->Render(shader_);
}