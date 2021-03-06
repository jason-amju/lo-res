// "LO RES" Game Prototype
// (c) Jason Colman 2017

#include <algorithm>
#include <cassert>
#include <iostream>
#include "lodepng.h"
#include "image.h"

const COLOUR_INDEX image::TRANSPARENT = 0;

void image::set_size(int w, int h)
{
  m_width = w;
  m_height = h;
  m_data.resize(w * h); 
}

void image::clear(COLOUR_INDEX c)
{
  for (COLOUR_INDEX& ch : m_data)
  {
    ch = c;
  }
}

void image::blit(image& dest, int dest_x, int dest_y) const
{
  blit_region(dest, dest_x, dest_y, 0, 0, m_width, m_height);
}

void image::blit_region(image& dest, int dest_x, int dest_y, 
  int src_x, int src_y, int src_w, int src_h) const
{
  // Blit this image onto dest.
  
  // Don't blit pixels which are off the left hand edge of dest.
  // Equivalent to:
  // int x_min = 0;
  // if (dest_x < 0)
  // {
  //   x_min = -dest_x;
  // }
  int x_min = std::max(0, -dest_x);

  // Don't blit pixels which are off the right hand edge of dest.
  // Equivalent to:
  // int x_max = m_width;
  // if (dest_x + m_width > dest.m_width)
  // {
  //   x_max = dest.m_width - dest_x;
  // }
  int x_max = std::min(src_w, dest.m_width - dest_x);

  // Same for top and bottom
  int y_min = std::max(0, -dest_y);
  int y_max = std::min(src_h, dest.m_height - dest_y);

  // Blit the region of this image which intersects the destination
  //  image.
  for (int x = x_min; x < x_max; x++)
  {
    for (int y = y_min; y < y_max; y++)
    {
      int dest_index = dest.index(x + dest_x, y + dest_y);
      COLOUR_INDEX col = get_colour(index(x + src_x, y + src_y));
      // Don't blit if the source colour is the transparent colour key.
      if (col != TRANSPARENT)
      {
        dest.set_colour(dest_index, col);
      }
    }
  }
}

bool image::load(const std::string& png_file_name, palette& pal)
{
  std::vector<unsigned char> data;
  unsigned int w = 0;
  unsigned int h = 0;
  unsigned nError = lodepng::decode(data, w, h, png_file_name);
  if (nError != 0)
  {
    std::cout << "Failed to load " << png_file_name << "\n";
    assert(0);
    return false;
  }

  set_size(m_width, m_height);

  // Add each RGB colour to palette. Set palette index in data,
  //  not the actual colour.
  int rgb_size = w * h * 4;
  for (int i = 0; i < rgb_size; i += 4)
  {
    int alpha = data[i + 3];
    colour col(data[i], data[i + 1], data[i + 2]);

    const int TRANSPARENT_LIMIT = 128;
    const colour MAGENTA(255, 0, 255);

    if (alpha < TRANSPARENT_LIMIT || col == MAGENTA)
    {
      m_data.push_back(TRANSPARENT);
    }
    else
    {
      int pal_index = pal.add_colour(col); 
      assert(pal_index < 0x100);
      char ch = pal_index & 0xff;
      m_data.push_back(ch);
    }
  }

  m_width = w;
  m_height = h;

  return true;
}



