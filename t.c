/* Tunguska - 64k Demo by Team210 at Solskogen 2019
 * Copyright (C) 2018 Alexander Kraus <nr4@z10.info>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

const char *demoname = "Tunguska/Team210";
unsigned int muted = 0.;

int _fltused = 0;

#include "common.h"


int load_fragment_shader(const char* const shader)
{
	int size = strlen(shader);
	int handle = glCreateShader(GL_FRAGMENT_SHADER);
	int program = glCreateProgram();

	glShaderSource(handle, 1, (GLchar **)&shader, &size);
	glCompileShader(handle);
	printf("---> Load shader:\n");
	debug(handle);
	glAttachShader(program, handle);
	glLinkProgram(program);
	printf("---> Load Program:\n");
	debugp(program);
	glUseProgram(program);

	return program;
}

void load_demo()
{
	// Load loading bar shader
	printf("++++ Creating Loading bar.\n");
	load_program = load_fragment_shader(load_frag);
	load_progress_location = glGetUniformLocation(load_program, LOAD_VAR_IPROGRESS);
	load_time_location = glGetUniformLocation(load_program, LOAD_VAR_ITIME);
	load_resolution_location = glGetUniformLocation(load_program, LOAD_VAR_IRESOLUTION);
	printf("++++ Loading bar created.\n");

	// Load post processing shader
	printf("++++ Creating Post Shader.\n");
	post_program = load_fragment_shader(post_frag);
	post_channel0_location = glGetUniformLocation(post_program, POST_VAR_ICHANNEL0);
	post_fsaa_location = glGetUniformLocation(post_program, POST_VAR_IFSAA);
	post_resolution_location = glGetUniformLocation(post_program, POST_VAR_IRESOLUTION);
	printf("++++ Post shader created.\n");

	// Create framebuffer for rendering first pass to
	glGenFramebuffers(1, &first_pass_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, first_pass_framebuffer);
	glGenTextures(1, &first_pass_texture);
	glBindTexture(GL_TEXTURE_2D, first_pass_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, first_pass_texture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Generate music framebuffer
	// Initialize sequence texture
	printf("sequence texture width is: %d\n", sequence_texture_size); // TODO: remove
	glGenTextures(1, &sequence_texture_handle);
	glBindTexture(GL_TEXTURE_2D, sequence_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sequence_texture_size, sequence_texture_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, sequence_texture);

	glGenFramebuffers(1, &snd_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, snd_framebuffer);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	unsigned int snd_texture;
	glGenTextures(1, &snd_texture);
	glBindTexture(GL_TEXTURE_2D, snd_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texs, texs, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, snd_texture, 0);

	// Music allocs
	nblocks1 = sample_rate * duration1 / block_size + 1;
	music1_size = nblocks1 * block_size;
	smusic1 = (float*)malloc(4 * music1_size);
	short *dest = (short*)smusic1;
	for (int i = 0; i < 2 * music1_size; ++i)
	{
		dest[i] = 0;
	}

	updateBar();

	// Load music shader
	LoadMusicThread(0);
	updateBar();

	// Load Logo 210 shader
//	LoadDecayingfactoryThread(0);
//	updateBar();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	LoadSymbols();
	LoadPrograms();

	LoadTextThread(0);
	updateBar();

	for (int music_block = 0; music_block < nblocks1; ++music_block)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, snd_framebuffer);
		glUseProgram(sfx_program);

		printf("Rendering SFX block %d/%d -> %le\n", music_block, nblocks1, .5 * (float)music_block / (float)nblocks1);
		double tstart = (double)(music_block*block_size);

		glViewport(0, 0, texs, texs);

		glUniform1f(sfx_volumelocation, 1.);
		glUniform1f(sfx_samplerate_location, (float)sample_rate);
		glUniform1f(sfx_blockoffset_location, (float)tstart);
		glUniform1i(sfx_texs_location, texs);
		glUniform1i(sfx_sequence_texture_location, 0);
		glUniform1f(sfx_sequence_width_location, sequence_texture_size);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sequence_texture_handle);

		quad();

		glReadPixels(0, 0, texs, texs, GL_RGBA, GL_UNSIGNED_BYTE, smusic1 + music_block * block_size);
		glFlush();

		unsigned short *buf = (unsigned short*)smusic1;
		short *dest = (short*)smusic1;
		if (!muted)
		{
			for (int j = 2 * music_block * block_size; j < 2 * (music_block + 1) * block_size; ++j)
			{
				dest[j] = (buf[j] - (1 << 15));
			}
		}
		else
		{
			for (int j = 2 * music_block * block_size; j < 2 * (music_block + 1) * block_size; ++j)
			{
				dest[j] = 0;
			}
		}
		updateBar();
	}

	glUseProgram(0);

	initialize_sound();
}

unsigned long __stdcall LoadMusicThread(void *lpParam)
{
    sfx_program = load_fragment_shader(sfx_frag);
    sfx_samplerate_location = glGetUniformLocation(sfx_program, SFX_VAR_ISAMPLERATE);
    sfx_blockoffset_location = glGetUniformLocation(sfx_program, SFX_VAR_IBLOCKOFFSET);
    sfx_volumelocation = glGetUniformLocation(sfx_program, SFX_VAR_IVOLUME);
    sfx_texs_location = glGetUniformLocation(sfx_program, SFX_VAR_ITEXSIZE);
    sfx_sequence_texture_location = glGetUniformLocation(sfx_program, SFX_VAR_ISEQUENCE);
    sfx_sequence_width_location = glGetUniformLocation(sfx_program, SFX_VAR_ISEQUENCEWIDTH);
    printf("++++ SFX shader created.\n");
    
    music_loading = 1;
    progress += .1 / NSHADERS; 
    
    return 0;
}

unsigned long __stdcall LoadLogo210Thread(void *lpParam)
{
    logo210_program = load_fragment_shader(logo210_frag);
    logo210_time_location =  glGetUniformLocation(logo210_program, LOGO210_VAR_ITIME);
    logo210_resolution_location = glGetUniformLocation(logo210_program, LOGO210_VAR_IRESOLUTION);
    printf("++++ Logo 210 shader created.\n");
    
    progress += .1 / NSHADERS;
    
    return 0;
}

// unsigned long __stdcall LoadDecayingfactoryThread(void *lpParam)
// {
//     decayingfactory_program = load_fragment_shader(decayingfactory_frag);
//     decayingfactory_time_location =  glGetUniformLocation(decayingfactory_program, DECAYINGFACTORY_VAR_ITIME);
//     decayingfactory_resolution_location = glGetUniformLocation(decayingfactory_program, DECAYINGFACTORY_VAR_IRESOLUTION);
//     printf("++++ Decaying factory shader created.\n");
//     
//     progress += .5 / NSHADERS;
//     
//     return 0;
// }

unsigned long __stdcall LoadTextThread(void *lpParam)
{
    text_program = load_fragment_shader(text_frag);
    text_time_location =  glGetUniformLocation(text_program, TEXT_VAR_ITIME);
    text_resolution_location = glGetUniformLocation(text_program, TEXT_VAR_IRESOLUTION);
    text_font_location= glGetUniformLocation(text_program, TEXT_VAR_IFONT);
    text_font_width_location = glGetUniformLocation(text_program, TEXT_VAR_IFONTWIDTH);
    text_channel0_location = glGetUniformLocation(text_program, TEXT_VAR_ICHANNEL0);
    printf("++++ Text shader created.\n");
    
    // Initialize font texture
    printf("font texture width is: %d\n", font_texture_size); // TODO: remove
    glGenTextures(1, &font_texture_handle);
    glBindTexture(GL_TEXTURE_2D, font_texture_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_texture_size, font_texture_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_texture);
    
    progress += .1 / NSHADERS;
    
    return 0;
}

void quad()
{
    glBegin(GL_QUADS);
    glVertex3f(-1, -1, 0);
    glVertex3f(-1, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, -1, 0);
    glEnd();
    glFlush();
}

void updateBar()
{
    glBindFramebuffer(GL_FRAMEBUFFER, first_pass_framebuffer);
    
    // Render first pass
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(load_program);
    glUniform2f(load_resolution_location, w, h);
    progress += .5 / nblocks1;
    glUniform1f(load_progress_location, progress);
    
    quad();
    
    // Render second pass (Post processing) to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, w, h);
    
    glUseProgram(post_program);
    glUniform2f(post_resolution_location, w, h);
    glUniform1f(post_fsaa_location, fsaa);
    glUniform1i(post_channel0_location, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, first_pass_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    quad();
    
    glBindTexture(GL_TEXTURE_2D, 0);

	flip_buffers();
}

// Pure opengl drawing code, essentially cross-platform
void draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, first_pass_framebuffer);
    
    float t = t_now;
    if(t > t_end)
	{
        ExitProcess(0);
	}
    
    if(scene_override)
    {
        if(override_index == 1)
        {
            glUseProgram(decayingfactory_program);
            glUniform1f(decayingfactory_iTime_location, t);
            glUniform2f(decayingfactory_iResolution_location, w, h);
        }
    }
    else
    {
        if(t < 9000.f)
        {
//             printf("iTime=%le\n", t);
//             printf("program: %d, timeloc: %d, resloc: %d\n", decayingfactory_program, decayingfactory_iTime_location, decayingfactory_iResolution_location);
            glUseProgram(fogforest_program);
            glUniform1f(fogforest_iTime_location, t);
            glUniform2f(fogforest_iResolution_location, w, h);
        }
        else
		{
			ExitProcess(0);
		}
    }

    quad();
    
    // Render text to buffer
    glUseProgram(text_program);
    glUniform2f(text_resolution_location, w, h);
    glUniform1f(text_font_width_location, font_texture_size);
    glUniform1f(text_time_location, t);
    glUniform1i(text_channel0_location, 0);
    glUniform1i(text_font_location, 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, first_pass_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, font_texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_texture_size, font_texture_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    quad();
    
    // Render second pass (Post processing) to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, w, h);
    
    glUseProgram(post_program);
    glUniform2f(post_resolution_location, w, h);
    glUniform1f(post_fsaa_location, fsaa);
    glUniform1i(post_channel0_location, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, first_pass_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    quad();
    
    glBindTexture(GL_TEXTURE_2D, 0);
}
