#include <GraphicalObjects/BodyReference.h>

BodyReference::BodyReference()
{
	m_body = std::make_shared<chrono::ChBody>();
	m_body->SetBodyFixed(true);
}

BodyReference::BodyReference(const BodyReference& another)
{
	*this = another;
}

BodyReference::BodyReference(BodyReference&& another) noexcept
{
	*this = std::move(another);
}

BodyReference& BodyReference::operator=(const BodyReference& another)
{
	if (this != &another)
	{
		m_body = another.m_body;
	}
	return *this;
}

BodyReference& BodyReference::operator=(BodyReference&& another) noexcept
{
	if (this != &another)
	{
		m_body = std::exchange(another.m_body, nullptr);
	}
	return *this;
}

const std::shared_ptr<chrono::ChBody> BodyReference::GetBody() const
{
	return m_body;
}
