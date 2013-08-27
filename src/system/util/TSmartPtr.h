/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 05. 8
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TSmartPtr.h
 *
 */


#ifndef _TSMARTPTR_H_
#define _TSMARTPTR_H_

#include <functional>
#include <stdexcept>
#include <cassert>
#include <string>

//==============================================
// TSmartPtr
//
template <typename T>
class TSmartPtr
{
    typedef T* PointerType;     /// type returned by operator->
    typedef T& ReferenceType;   /// type returned by operator*

	typedef const PointerType ConstPointerType;
	typedef const ReferenceType ConstReferenceType;

private:

	PointerType	ptr;
	int	*ref;

public:

	//--------------------------------------------------
	// ctors & dtors
	//
	TSmartPtr() {
		ptr = NULL;
		ref = NULL;
    }

    explicit TSmartPtr(const PointerType p) {
		ptr = p;
		if (ptr != NULL) ref = new int(1);
		else ref = NULL;
    }

	template <typename U>
    explicit TSmartPtr(const U p) {
		ptr = (PointerType) p;
		if (ptr != NULL) ref = new int(1);
		else ref = NULL;
    }

	TSmartPtr(const TSmartPtr<T>& rhs) {
		ptr = NULL;
		ref = NULL;
		ptrAcquire(rhs);
    }

 	template< typename U >
	TSmartPtr(TSmartPtr<U>& rhs) {
		ptr = NULL;
		ref = NULL;
		ptrAcquire(rhs);
	}

	~TSmartPtr()  {
		ptrRelease();
    }

	//
	//
	void incRef() { 
		if (ref) ++(*ref); 
	};
	void decRef() { 
		if (ref) --(*ref); 
	};
	bool isRef() { 
		if (ref) return *ref != 0; 
		else return false;
	};
	int* refGet() const { 
		return ref; 
	};
	void refSet(int *ref) { 
		ref = ref; 
	};

	PointerType	ptrGet() const { 
		return ptr; 
	};

	void ptrSet(const PointerType p) { 
		ptr = p; 
	};

	void ptrAcquire(const TSmartPtr<T>& rhs) { 
		ptrRelease();

		ptr = rhs.ptrGet();
		ref = rhs.refGet();
		incRef();
	}

	template< typename U >
	void ptrAcquire(TSmartPtr<U>& rhs) { 
		ptrRelease();

		ptr = (PointerType) rhs.ptrGet();
		ref = rhs.refGet();
		incRef();
	}

	bool ptrRelease()  {
		decRef();
		if (isRef()) {
			return false;
		}
		else {
			if (ref != NULL) delete ref;
			if (ptr != NULL) delete ptr;
			return true;
		}
	}


	//------------------------------------
	// ref, deref
	//
	PointerType operator->()  {
        return ptrGet();
    }

    ConstPointerType operator->() const {
        return ptrGet();
    }

    ReferenceType operator*() {
        return *(ptrGet());
    }

    ConstReferenceType operator*() const {
        return *(ptrGet());
    }

	//--------------------------------------
	// casting
	//

	template <typename newType>
	operator TSmartPtr<newType>() {
		return TSmartPtr<newType>(ptrGet());
	}

	//-----------------------------------------
	// operator overloading
	//
    TSmartPtr& operator=(TSmartPtr<T>& rhs) {
		if (this != &rhs) {
			ptrAcquire(rhs);
		}
        return *this;
    }

    bool operator!() { 
		PointerType ptr = ptrGet();
		return  ptr == NULL; 
	}

	friend bool operator==(const TSmartPtr<T>& lhs, T* rhs) { 
		return lhs.ptrGet() == rhs; 
	}

	friend bool operator==(T* lhs, const TSmartPtr<T>& rhs) { 
		return rhs == lhs; 
	}

	friend bool operator!=(const TSmartPtr<T>& lhs, T* rhs) { 
		return !(lhs == rhs); 
	}

	friend bool operator!=(T* lhs, const TSmartPtr<T>& rhs) { 
		return rhs != lhs; 
	}

	friend bool operator<(const TSmartPtr<T>& lhs,  T* rhs) {
		return (lhs.ptrGet() < rhs);
	}

	friend bool operator<(T* lhs, const TSmartPtr<T>& rhs) {
		return (rhs.ptrGet() < lhs);
	}

	friend bool operator>(const TSmartPtr<T>& lhs,  T* rhs) { 
		return rhs < lhs; 
	}

	friend bool operator>(T* lhs, const TSmartPtr<T>& rhs) { 
		return rhs < lhs; 
	}

	friend bool operator<=(const TSmartPtr<T>& lhs, T* rhs) { 
		return !(rhs < lhs); 
	}

	friend bool operator<=(T* lhs, const TSmartPtr<T>& rhs) { 
		return !(rhs < lhs); 
	}

	friend bool operator>=(const TSmartPtr<T>& lhs, T* rhs) { 
		return !(lhs < rhs); 
	}

	friend bool operator>=(T* lhs, const TSmartPtr<T>& rhs) { 
		return !(lhs < rhs); 
	}
 };


////////////////////////////////////////////////////////////////////////////////
// friends
////////////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline bool operator==(const TSmartPtr<T>& lhs, U* rhs) { 
	return lhs.ptrGet() == rhs; 
}

template <typename T, typename U>
inline bool operator==(U* lhs, const TSmartPtr<T>& rhs) { 
	return rhs == lhs; 
}

template <typename T, typename U>
inline bool operator!=(const TSmartPtr<T>& lhs, U* rhs) { 
	return !(lhs == rhs); 
}

template <typename T, typename U>
inline bool operator!=(U* lhs, const TSmartPtr<T>& rhs) { 
	return rhs != lhs; 
}

template <typename T, typename U>
inline bool operator<(const TSmartPtr<T>& lhs,  U* rhs) {
    return (lhs.ptrGet() < rhs);
}

template <typename T, typename U>
inline bool operator<(U* lhs, const TSmartPtr<T>& rhs) {
    return (rhs.ptrGet() < lhs);
}

template <typename T, typename U>
inline bool operator>(const TSmartPtr<T>& lhs,  U* rhs) { 
	return rhs < lhs; 
}

template <typename T, typename U>
inline bool operator>(U* lhs, const TSmartPtr<T>& rhs) { 
	return rhs < lhs; 
}

template <typename T, typename U>
inline bool operator<=(const TSmartPtr<T>& lhs, U* rhs) { 
	return !(rhs < lhs); 
}

template <typename T, typename U>
inline bool operator<=(U* lhs, const TSmartPtr<T>& rhs) { 
	return !(rhs < lhs); 
}

template <typename T, typename U>
inline bool operator>=(const TSmartPtr<T>& lhs, U* rhs) { 
	return !(lhs < rhs); 
}

template <typename T, typename U>
inline bool operator>=(U* lhs, const TSmartPtr<T>& rhs) { 
	return !(lhs < rhs); 
}

////////////////////////////////////////////////////////////////////////////////
///  specialization of std::less for TSmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

namespace std
{
	template <typename T>
    struct less< TSmartPtr<T> >
        : public binary_function<TSmartPtr<T>, TSmartPtr<T>, bool>
    {
        bool operator()(const TSmartPtr<T>& lhs, const TSmartPtr<T>& rhs) const { 
			return less<T*>()(lhs.ptrGet(), rhs.ptrGet()); 
		}
    };
}

#endif // end file guardian

