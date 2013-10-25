/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "texture.h"
#include <glog/logging.h>

namespace handy 
{
  Texture::Texture(GLenum TextureTarget, const std::string& FileName)
  {
      textureTarget = TextureTarget;
      fileName      = FileName;
      image         = nullptr;
  }

  bool Texture::load()
  {
      try {
          image = new Magick::Image(fileName);
          image->write(&blob, "RGBA");
      }
      catch (Magick::Error& Error) {
          LOG(ERROR) << "Error loading texture '" << fileName;
          LOG(ERROR) << "': " << Error.what() << std::endl;
          return false;
      }

      // Allocate space for textures
      glGenTextures(1, &textureObject);

      // Bind the texture object
      glBindTexture(textureTarget, textureObject);

      // And make a 2D texture
      glTexImage2D(textureTarget, 
                   0, 
                   GL_RGB,               // Internal format
                   image->columns(), 
                   image->rows(), 
                   0, 
                   GL_RGBA,              // Image format
                   GL_UNSIGNED_BYTE,     // Image depth
                   blob.data());
      
      // Set texture options
      glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      return true;
  }

  void Texture::bind(GLenum textureUnit)
  {
      glActiveTexture(textureUnit);
      glBindTexture(textureTarget, textureObject);
  }
};
