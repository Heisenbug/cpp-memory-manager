#ifndef MMALLOCATION_TRACKER_H_INCLUDE_GUARD
#define MMALLOCATION_TRACKER_H_INCLUDE_GUARD

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>

#include <iostream>

#include "MMPreprocDirectives.h"

namespace MM
{
	class AllocationTracker
	{
	private:

		struct TrackingInfo
		{
			TrackingInfo() 
				: mSize(0), mLine(0) { }
			TrackingInfo(size_t size, const char* category, const char* file, size_t line, const char* func) 
				: mSize(size), mLine(line) 
			{
				if (category)
					mCategory = category;
				if (file)
					mFile = file;
				if (func)
					mFunc = func;
			}

			size_t		mSize;
			std::string mCategory;

			std::string mFile;
			size_t		mLine;
			std::string mFunc;
		};

	public:

		~AllocationTracker()
		{
			ReportLeaks();
		}

		static AllocationTracker& GetInstance()
		{
			static AllocationTracker tracker;
			return tracker;
		}

		size_t GetTotalMemoryAllocated() const
		{
			return mTotalAllocations;
		}

		const std::string& GetOutputFileName() const
		{
			return mOutputFileName;
		}

		void RecordAllocation(void* p, size_t size, const char* category, const char* file,
			size_t line, const char* func)
		{
			assert(mAllocationMap.find(p) == mAllocationMap.end() && "Double allocation with same address.");
			mAllocationMap[p] = TrackingInfo(size, category, file, line, func);
			mTotalAllocations += size;
		}

		void RecordDellocation(void* p)
		{
			AllocMap::iterator it = mAllocationMap.find(p);
			assert(it != mAllocationMap.end() && "Unable to locate allocation unit.");
			mTotalAllocations -= it->second.mSize;
			mAllocationMap.erase(it);
		}

	private:

		// Private constructor
		AllocationTracker()	
			: mOutputFileName(MM_MEM_LEAKS_FILE_NAME), mTotalAllocations(0) { }

		void ReportLeaks()
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);

			if (mAllocationMap.empty())
			{
				ss << "MemoryManager: No memory leaks detected. Cool!" << std::endl;
			}
			else
			{
				ss << "MemoryManager: Detected memory leaks. Noob!" << std::endl;
				ss << "MemoryManager: " << mAllocationMap.size() << " allocation(s) with total " << mTotalAllocations << " bytes." << std::endl;

				for (AllocMap::const_iterator it = mAllocationMap.begin(); it != mAllocationMap.end(); ++it)
				{
					const TrackingInfo& trackingInfo = it->second;
					if (!trackingInfo.mFile.empty())				
						ss << trackingInfo.mFile;
					else
						ss << "(unknown source)";

					ss << " : (" << trackingInfo.mLine << ") : {" << trackingInfo.mSize << " bytes of ";
					
					if (!trackingInfo.mCategory.empty())
						ss << trackingInfo.mCategory;
					else 
						ss << "(unknown category)";
				
					ss << "}" << ", ";

					if (!trackingInfo.mFunc.empty())
						ss << trackingInfo.mFunc;
					else
						ss << "(unknown function).";
					
					ss << std::endl;
				}
			}

			// Write information
			std::fstream of(mOutputFileName.c_str(), std::fstream::out);
			of << ss.str();
			of.close();
		}

		typedef std::map<void*, TrackingInfo> AllocMap;

		std::string mOutputFileName;

		size_t		mTotalAllocations;

		AllocMap	mAllocationMap;
	};
}

#endif // MMALLOCATION_TRACKER_H_INCLUDE_GUARD