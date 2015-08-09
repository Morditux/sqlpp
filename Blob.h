/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2015 Morditux
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File:   Blob.h
 * Author: Morditux
 *
 * Created on January 6, 2015, 8:16 PM
 */



#ifndef BLOB_H
#define	BLOB_H

#include <memory>
#include <stdint.h>
#include <mutex>

namespace SQLPP
{
    class Blob;
    /* Easy way to create reference counted data */
    class _BlobData 
    {
        friend Blob;
    public:
        _BlobData() : data(nullptr),size(0) {}
        ~_BlobData() 
        {
            ::free(data);
        }
    private:
        std::recursive_mutex mutex;
        int32_t size;
        char * data;
    };

    class Blob
    {
    public:
        Blob(int32_t size, const char * data);
        
        virtual ~Blob();
        
        int32_t size() const;
        
        const char * data() const;
        
    private:
        
        std::shared_ptr<_BlobData> d;
    };

}
#endif	/* BLOB_H */

