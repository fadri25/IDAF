#include "framebuffer.h"


#include "includegl.h"
#include "gldebug.h"


Framebuffer::Framebuffer() {

}

Framebuffer::Framebuffer(int w, int h) {

	GL_CALL(glGenFramebuffers(1, &id));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.getID(), 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Failed to create Framebuffer\n");
		__debugbreak();
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &id);
}


void Framebuffer::bind() const {
	tex.bind();
}

void Framebuffer::bindAsTarget() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}
