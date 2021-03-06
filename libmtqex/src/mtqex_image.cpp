/*
	Copyright (C) 2013-2018 Mark Tyler

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program in the file COPYING.
*/

#include "private.h"



mtQEX::Image::Image ()
	:
	image		(),
	zoom		( 1 )
{
	setAlignment ( Qt::AlignHCenter | Qt::AlignVCenter );

	area = new ImageArea ( this );
	setWidget ( area );

	setBackgroundRole ( QPalette::Dark );
}

mtQEX::Image::~Image ()
{
	delete image;
	image = NULL;
}

void mtQEX::Image::setImage (
	mtPixy::Image	* const	im
	)
{
	delete image;
	image = im;

	if ( im )
	{
		area->setGeometry ( 0, 0,
			image->get_width () * zoom,
			image->get_height () * zoom );
	}
	else
	{
		area->setGeometry ( 0, 0, 0, 0 );
	}

	// Note: both of these are needed to cover all scenarios
	area->update ();
	area->updateGeometry ();
}

mtPixy::Image * mtQEX::Image::getImage ()
{
	return image;
}

int mtQEX::Image::setZoom (
	int	const	z
	)
{
	if ( z < 1 || z > 100 )
	{
		return 1;
	}

	if ( z != zoom )
	{
		zoom = z;

		if ( image )
		{
			area->setGeometry ( 0, 0,
				image->get_width () * zoom,
				image->get_height () * zoom );

			area->update ();
			area->updateGeometry ();
		}
	}

	return 0;
}

int mtQEX::Image::getZoom ()
{
	return zoom;
}

void mtQEX::Image::update ()
{
	area->update ();
}

mtQEX::ImageArea::ImageArea (
	Image		* const	par
	)
	:
	qi		( par )
{
	setAttribute ( Qt::WA_OpaquePaintEvent );
	setAttribute ( Qt::WA_NoSystemBackground );
}

void mtQEX::ImageArea::paintEvent (
	QPaintEvent	* const	ev
	)
{
	mtPixy::Image	* destImage, * srcImage;
	int		px, py, pw, ph;


	px = ev->rect ().x ();
	py = ev->rect ().y ();
	pw = ev->rect ().width ();
	ph = ev->rect ().height ();

	destImage = mtPixy::Image::create ( mtPixy::Image::TYPE_RGB, pw, ph );
	unsigned char * const rgb = destImage->get_canvas ();

	if ( ! rgb )
	{
		delete destImage;
		return;
	}

	if (	qi						&&
		(srcImage = qi->getImage () )
		)
	{
		mtPixy::Palette	* const	pal = srcImage->get_palette ();

		if ( pal )
		{
			int const zoom = qi->getZoom ();
			mtPixy::Color const * const col = pal->get_color ();

			srcImage->blit_rgb ( col, rgb, -px, -py, pw, ph, zoom );
		}
	}

	QImage * qim = new QImage ( (const uchar *)rgb, pw, ph, pw * 3,
		QImage::Format_RGB888 );

	if ( qim )
	{
		QPainter p ( this );
		p.drawImage ( QPoint ( px, py ), qim[0] );

		delete qim;		// Delete before rgb image destroy
		qim = NULL;
	}

	delete destImage;
	destImage = NULL;
}

