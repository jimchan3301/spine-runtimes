/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated September 24, 2021. Replaces all prior versions.
 *
 * Copyright (c) 2013-2021, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/Sequence.h>
#include <spine/extension.h>

_SP_ARRAY_IMPLEMENT_TYPE(spTextureRegionArray, spTextureRegion*)

static int nextSequenceId = 0;

spSequence *spSequence_create(int start, int digits, int setupIndex, int numRegions) {
	spSequence *self = NEW(spSequence);
	self->id = nextSequenceId++;
	self->start = start;
	self->digits = digits;
	self->setupIndex = setupIndex;
	self->regions = spTextureRegionArray_create(numRegions);
	spTextureRegionArray_setSize(self->regions, numRegions);
	return self;
}

void spSequence_dispose(spSequence *self) {
	FREE(self->regions);
	FREE(self);
}

spSequence *spSequence_copy(spSequence *self) {
	int i = 0;
	spSequence *copy = spSequence_create(self->start, self->digits, self->setupIndex, self->regions->size);
	for (; i < self->regions->size; i++)
		copy->regions->items[i] = self->regions->items[i];
	copy->start = self->start;
	copy->digits = self->digits;
	copy->setupIndex = self->setupIndex;
	return copy;
}

void spSequence_apply(spSequence *self, spSlot *slot, spAttachment *attachment) {
	int index = slot->sequenceIndex;
	spTextureRegion *region = NULL;
	if (index == -1) index = self->setupIndex;
	if (index >= (int) self->regions->size) index = self->regions->size - 1;
	region = self->regions->items[index];

	if (attachment->type == SP_ATTACHMENT_REGION) {
		spRegionAttachment *regionAttachment = (spRegionAttachment*)attachment;
		if (regionAttachment->region != region) {
			regionAttachment->rendererObject = region;
			regionAttachment->region = region;
			spRegionAttachment_updateRegion(regionAttachment);
		}
	}

	if (attachment->type == SP_ATTACHMENT_MESH) {
		spMeshAttachment *meshAttachment = (spMeshAttachment*)attachment;
		if (meshAttachment->region != region) {
			meshAttachment->rendererObject = region;
			meshAttachment->region = region;
			spMeshAttachment_updateRegion(meshAttachment);
		}
	}
}

void spSequence_getPath(const char* basePath, int index, char* path) {
	fix me
}
