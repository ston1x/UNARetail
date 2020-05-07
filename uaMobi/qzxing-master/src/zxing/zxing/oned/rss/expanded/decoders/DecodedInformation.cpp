#include "DecodedInformation.h"
#ifndef Q_NULLPTR
#define Q_NULLPTR nullptr
#endif
namespace zxing {
	namespace oned {
		namespace rss {
			DecodedInformation::DecodedInformation(const DecodedInformation* other)
				: DecodedObject(other == Q_NULLPTR ? 0 : other->m_newPosition),
				m_newString(other == Q_NULLPTR ? String("") : other->m_newString)
			{
				m_newString = other == Q_NULLPTR ? String("") : other->m_newString;
				m_remaining = other == Q_NULLPTR ? false : other->m_remaining;
				m_remainingValue = other == Q_NULLPTR ? 0 : other->m_remainingValue;
			}

			DecodedInformation::DecodedInformation(int newPosition, String newString)
				: DecodedObject(newPosition),
				m_newString(newString),
				m_remainingValue(0),
				m_remaining(false)
			{
			}

			DecodedInformation::DecodedInformation(int newPosition, String newString, int remainingValue)
				: DecodedObject(newPosition),
				m_newString(newString),
				m_remainingValue(remainingValue),
				m_remaining(true)
			{
			}

			String DecodedInformation::getNewString() const
			{
				return m_newString;
			}

			bool DecodedInformation::isRemaining() const
			{
				return m_remaining;
			}

			int DecodedInformation::getRemainingValue() const
			{
				return m_remainingValue;
			}
		}
	}
}