    #pragma once
     
    #include <Windows.h>
    #include <stdexcept>
     
    class SharedMemoryStream
    {
    public:
    	SharedMemoryStream(const wchar_t* nameFormat);
    	~SharedMemoryStream();
     
    	void PutFront(const void* source, DWORD size);
    	void Get(void* destination, DWORD size);
     
    	HANDLE GetAvailableHandle() const;
    	HANDLE GetWrittenHandle() const;
    	HANDLE GetMutexHandle() const;
     
    	DWORD GetUnusedCapacity() const;
    	DWORD GetCapacity() const;
    	DWORD GetSize() const;
     
    private:
    	struct StreamHeader
    	{
    		DWORD BufferStartIndex;
    		DWORD BufferEndIndex;
    		DWORD BufferCapacity;
    		DWORD BufferSize;
    	};
     
    	HANDLE FileMappingHandle;
    	HANDLE AvailableHandle;
    	HANDLE WrittenHandle;
    	HANDLE MutexHandle;
     
    	LPVOID FileMappingView;
     
    	StreamHeader* Header;
    	char* Buffer;
    };