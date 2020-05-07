#include "BlockParsedResult.h"
#ifndef Q_NULLPTR
#define Q_NULLPTR nullptr
#endif
namespace zxing {
	namespace oned {
		namespace rss {
			BlockParsedResult::BlockParsedResult(bool finished)
				: m_decodedInformation(Q_NULLPTR), m_finished(finished)
			{
			}

			BlockParsedResult::BlockParsedResult(const DecodedInformation& information, bool finished)
			{
				m_finished = finished;
				m_decodedInformation = information;
			}

			DecodedInformation BlockParsedResult::getDecodedInformation()
			{
				return m_decodedInformation;
			}

			bool BlockParsedResult::isFinished()
			{
				return m_finished;
			}
		}
	}
}