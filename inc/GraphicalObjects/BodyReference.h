#pragma once

#include <memory>

#include <chrono/physics/ChBody.h>

class BodyReference
{
public:
	BodyReference();
	
	BodyReference(const BodyReference& another);
	BodyReference(BodyReference&& another) noexcept;

	BodyReference& operator=(const BodyReference& another);
	BodyReference& operator=(BodyReference&& another) noexcept;

	~BodyReference() = default;

	const std::shared_ptr<chrono::ChBody> GetBody() const;

private:
	std::shared_ptr<chrono::ChBody> m_body;
};