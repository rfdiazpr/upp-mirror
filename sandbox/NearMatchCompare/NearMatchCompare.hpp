/*
 * NearMatchAlgorithm.h
 *
 *  Created on: 17 juil. 2009
 *      Author: didier
 */

#ifndef NEAR_MATCH_COMPARE_H_
#define NEAR_MATCH_COMPARE_H_

class NearMatchCompare {
	private:
	
	public:
	
	template <class STRING_TYPE>
	static inline int correlation(const STRING_TYPE& A, int Al, const STRING_TYPE& B, int Bl)
	{
		const int deltaL = Al - Bl;
		int As, Bs, intersectLength;
		int matchPatternMinLength = Upp::fceil(Upp::max(Bl, Al)/3.0);
		int res=0;
	
		for (int offset = (matchPatternMinLength-Bl); offset <= (Al-matchPatternMinLength); offset++ )
		{
			// Determinate comparison Range
			if (offset < 0)
			{
				As = 0;
				Bs = -offset;
				if (offset < deltaL) intersectLength = offset + Bl;
				else                 intersectLength = Al;
			}
			else
			{
				As = offset;
				Bs = 0;
				intersectLength = Al - offset;
				if (offset <= deltaL) intersectLength = Bl;
				else                  intersectLength = Al-offset;
			}
	
			// Do actual comparison
			int subRes = 0;
			for (int c = 0; c<intersectLength; c++)
			{
				if( A[As+c] == B[Bs+c])
				{
					++subRes;
				}
			}
			res += subRes * subRes;
		}
	
		// Take in account string length
		if ( deltaL > 0 )
		{
			res -= deltaL;
		}
		else
		{
			res += deltaL;
		}
		return res;
	}
	
	template <class STRING_TYPE, class HIST_TYPE>
	static int correlation(const STRING_TYPE& A, int Al, const STRING_TYPE& B, int Bl, HIST_TYPE& aHistogram, HIST_TYPE& bHistogram)
	{
		// Histogram initialisation
		for (int c = 0; c<Al; c++)
		{
			aHistogram[c] = 0;
		}
	
		for (int c = 0; c<Bl; c++)
		{
			bHistogram[c] = 0;
		}
	
		const int deltaL = Al - Bl;
		int As, Bs, intersectLength;
		int matchPatternMinLength = Upp::max(2, Upp::fceil(Upp::max(Bl, Al)/3.0));
		int res=0;
	
		for (int offset = (matchPatternMinLength-Bl); offset <= (Al-matchPatternMinLength); offset++ )
		{
			// Determinate comparison Range
			if (offset < 0)
			{
				As = 0;
				Bs = -offset;
				if (offset < deltaL) intersectLength = offset + Bl;
				else                 intersectLength = Al;
			}
			else
			{
				As = offset;
				Bs = 0;
				intersectLength = Al - offset;
				if (offset <= deltaL) intersectLength = Bl;
				else                  intersectLength = Al-offset;
			}
	
			int subRes = 0;
			// Do actual comparison
			for (int c = 0; c<intersectLength; c++)
			{
				if( A[As+c] == B[Bs+c]) ++subRes;
			}
			if (subRes >= matchPatternMinLength)
			{
				res += subRes;
	
				// Create histogram
				for (int c = 0; c<intersectLength; c++)
				{
					if( A[As+c] == B[Bs+c])
					{
						aHistogram[As+c] += subRes;
						bHistogram[Bs+c] += subRes;
					}
				}
			}
		}
	
		// Take in account string length
		if ( deltaL > 0 )
		{
			res -= deltaL;
		}
		else
		{
			res += deltaL;
		}
		return res;
	}
	
	
	static inline int correlation(const Upp::String& a, const Upp::String& b)
	{
		return correlation(a, a.GetLength(),  b, b.GetLength());
	}
	
	template <class STRING_TYPE>
	static inline bool isNearMatch(const STRING_TYPE& a, int Al, const STRING_TYPE& b, int Bl)
	{
		if (correlation(a, Al, b, Bl) >= Upp::fceil( (correlation(a, Al, a, Al) + correlation(b, Bl, b, Bl) )*4.0/19.0) ) return true;
		return false;
	}
	
	template <class STRING_TYPE, class HIST_TYPE>
	static inline bool isNearMatch(const STRING_TYPE& a, int Al, const STRING_TYPE& b, int Bl, HIST_TYPE& Ahist, HIST_TYPE& Bhist)
	{
		if (correlation(a, Al, b, Bl, Ahist, Bhist) >= Upp::max(2,  Upp::fceil(Upp::max(Al, Bl)*3.0/5.0))) return true;
		return false;
	}
	
	
	
	static inline bool isNearMatch(const Upp::String& a, const Upp::String& b)
	{
		return isNearMatch(a, a.GetLength(),  b, b.GetLength());
	}
	
	template <class HIST_TYPE>
	static inline bool isNearMatch(const Upp::String& a, const Upp::String& b, HIST_TYPE& Ahist, HIST_TYPE& Bhist )
	{
		return isNearMatch(a, a.GetLength(),  b, b.GetLength(), Ahist, Bhist);
	}
}; // namespace NearMatchCompare


#endif /* NEAR_MATCH_COMPARE_H_ */
