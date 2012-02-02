#include "OpenGLBuffer.h"

OpenGLBuffer::OpenGLBuffer()
{
	valid = false;
}

OpenGLBuffer::OpenGLBuffer( void* pData, const unsigned int data_size, GLenum buffer_type, GLenum usage )
{
	//Set the usage of our vertex buffer object (is is dynamically frequently updated buffer or fairly static once created)
	vbo_buffer_type = buffer_type;
	vbo_usage = usage;
	vbo_access = GL_WRITE_ONLY_ARB;
	this->data_size = data_size;
	

	//vboUsage = (Usage & OGLUSAGE_DYNAMIC ) ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;

	//Set the access type of our vertex buffer object
	//vboAccess = ( Usage & OGLUSAGE_WRITEONLY ) ? GL_WRITE_ONLY_ARB : GL_READ_WRITE_ARB;
	
	//create a new vertex buffer object and activate it so we may define its size and usage
	glGenBuffers( 1, &vbo_id );	
	glBindBuffer( vbo_buffer_type, vbo_id );	

	glBufferData( vbo_buffer_type, data_size, pData, vbo_usage);

	//now that the vertex buffer object has been created, unbind it
	glBindBuffer( vbo_buffer_type, 0 );

	valid = true;

}

void* OpenGLBuffer::Map()
{
//Bind our vertex buffer object
	glBindBuffer( vbo_buffer_type, vbo_id );
	
	//If the discard flag was set, we can quickly invalidate the data stored on the GPU by nulling it
	//before mapping the buffer to the client side. This allows the driver to avoid stalls due to 
	//attempting to maintain a copy of the old vertex buffer
	//if( Flags & OGLLOCK_DISCARD )
	//	glBufferDataARB( GL_ARRAY_BUFFER_ARB, SizeToLock, 0, GL_DYNAMIC_DRAW_ARB);

	//Map our vertex buffer object in GPU memory into client-side memory so the CPU can read/write to it.
	void* pMappedData = glMapBuffer( vbo_buffer_type, vbo_access );

	//Unbind our vertex buffer object
	glBindBuffer( vbo_buffer_type, 0 );

	//Return our success
	return pMappedData;
}

bool OpenGLBuffer::Unmap()
{
	//Bind our vertex buffer object.
	glBindBuffer( vbo_buffer_type, vbo_id );
	
	//Unmap our vertex data from client-side memory back into GPU memory
	GLboolean result = glUnmapBuffer( vbo_buffer_type );

	//Unbind our vertex buffer object.
	glBindBuffer( vbo_buffer_type, 0 );

	//return our success
	return result == GL_TRUE;

}

bool OpenGLBuffer::SetData( void* pData  )
{
	void* pMappedData = Map();
	memcpy( pMappedData, pData, data_size ); 
	Unmap();
	return true;
}