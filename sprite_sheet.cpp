// "LO RES" Game Prototype
// (c) Jason Colman 2017

#include <algorithm>
#include <cassert>
#include <iostream>
#include "sprite_sheet.h"

pix_int_result pixel_intersect(
  const sprite_sheet& ss1, int cell1, int x1, int y1, 
  const sprite_sheet& ss2, int cell2, int x2, int y2)
{
  // Cell sizes for both sprites - TODO member vars
  int w1 = ss1.m_width / ss1.m_cells_x;
  int h1 = ss1.m_height / ss1.m_cells_y;
  int w2 = ss2.m_width / ss2.m_cells_x;
  int h2 = ss2.m_height / ss2.m_cells_y;

std::cout 
  << "Sprite 1: cell w: "  << w1 << " h: " << h1 
  << " Sprite 2: cell w: " << w2 << " h: " << h2 << "\n";

  // Overlapping range of coords
  int xmin = std::max(x1, x2);
  int xmax = std::min(x1 + w1, x2 + w2);
  int ymin = std::max(y1, y2);
  int ymax = std::min(y1 + h1, y2 + h2);

std::cout
  << "Overlap: xmin: " << xmin << " xmax: " << xmax
  << " ymin: " << ymin << " ymax: " << ymax << "\n";

  // Bail if no overlapping region
  if (xmin >= xmax || ymin >= ymax)
  {
std::cout << "NO OVERLAP\n";
    return pix_int_result::NO_AND_DISJOINT;
  }

  // Find region on both sprite sheets
  // Find top-left of cell for each sprite
  int cell_x1 = cell1 % ss1.m_cells_x * w1;
  int cell_y1 = cell1 / ss1.m_cells_x * h1;
  int cell_x2 = cell2 % ss2.m_cells_x * w2;
  int cell_y2 = cell2 / ss2.m_cells_x * h2;

std::cout << "Top left cell coord: "
  << "cell_x1: " << cell_x1 << " cell_y1: " << cell_y1 
  << " cell_x2: " << cell_x2 << " cell_y2: " << cell_y2 << "\n";

  // Check correspondong pixels for non-transparent colour index
  int nx = xmax - xmin;
  int ny = ymax - ymin;
  for (int x = 0; x <nx; x++)
  {
    for (int y = 0; y < ny; y++) // better to swap for cache?
    {
      int px1 = cell_x1 + x + xmin - x1;
      int py1 = cell_y1 + y + ymin - y1;
      int px2 = cell_x2 + x + xmin - x2;
      int py2 = cell_y2 + y + ymin - y2;

std::cout << "Testing pixel pairs: "
  << "(" << px1 << ", " << py1 << ") and "
  << "(" << px2 << ", " << py2 << ")\n";

      COLOUR_INDEX c1 = ss1.get_colour(ss1.index(px1, py1));
      COLOUR_INDEX c2 = ss2.get_colour(ss2.index(px2, py2));

      if (c1 != image::TRANSPARENT && c2 != image::TRANSPARENT)
      {
std::cout << "Both non-transparent, HIT!\n";
        return pix_int_result::YES_COLLIDE;
      } 
    }
  }
std::cout << "No overlapping opaque pixels.\n";

  return pix_int_result::NO_AND_NOT_DISJOINT;
}

void sprite_sheet::draw_cell(image& dest, int cell, int dest_x, int dest_y)
{
  // Draw cell at pos on screen
  // Calc cell x, y, w, h

  int cell_w = m_width / m_cells_x;
  int cell_h = m_height / m_cells_y;

  // Get top left of current cell (m_cell)
  assert(cell < m_cells_x * m_cells_y);
  int cell_x = cell % m_cells_x * cell_w;
  int cell_y = cell / m_cells_x * cell_h;

  blit_region(dest, dest_x, dest_y,
    cell_x, cell_y, cell_w, cell_h);
}

