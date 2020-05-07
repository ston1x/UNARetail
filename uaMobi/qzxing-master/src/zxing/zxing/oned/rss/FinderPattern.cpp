#include "FinderPattern.h"
#ifndef Q_NULLPTR
#define Q_NULLPTR nullptr
#endif
namespace zxing {
	namespace oned {
		namespace rss {
			FinderPattern::FinderPattern(int value, std::vector<int> startEnd, int start, int end, int rowNumber)
				: m_value(value),
				m_startEnd(startEnd)
			{
				ArrayRef< Ref<ResultPoint> > resultPoints(2);
				resultPoints[0] = Ref<ResultPoint>(new OneDResultPoint(start, rowNumber));
				resultPoints[1] = Ref<ResultPoint>(new OneDResultPoint(end, rowNumber));
				m_resultPoints = resultPoints;
			}

			FinderPattern::FinderPattern(const FinderPattern* other)
			{
				m_value = other != Q_NULLPTR ? other->m_value : 0;
				m_startEnd = other != Q_NULLPTR ? other->m_startEnd : std::vector<int>();
				m_resultPoints = other != Q_NULLPTR ? other->m_resultPoints : Q_NULLPTR;
			}

			int FinderPattern::getValue() const
			{
				return m_value;
			}

			std::vector<int>& FinderPattern::getStartEnd()
			{
				return m_startEnd;
			}

			ArrayRef<Ref<ResultPoint> >& FinderPattern::getResultPoints()
			{
				return m_resultPoints;
			}

			bool FinderPattern::equals(const FinderPattern& other) const
			{
				return m_value == other.m_value;
			}

			int FinderPattern::hashCode() const
			{
				return m_value;
			}

			bool FinderPattern::isValid() const
			{
				return m_startEnd.size() != 0;
			}
		}
	}
}