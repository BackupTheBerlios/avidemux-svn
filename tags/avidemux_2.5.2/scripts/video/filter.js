include("video/functions.js");

function resizeAndFillVideo(targetX, targetY, sourceRatio, destinationRatio)
{
    var app = new Avidemux();
    var filters = app.video.appliedFilters;
    var width, height, fps1000;

    if (filters.length > 0)
    {
        var lastFilter = app.video.appliedFilters[filters.length - 1];

        width = lastFilter.width;
        height = lastFilter.height;
        fps1000 = lastFilter.fps1000;
    }
    else
    {
        width = app.video.width;
        height = app.video.height;
        fps1000 = app.video.fps1000;
    }

    // Resizing
    var scaledWidth = rescaleVideoDimension(width, sourceRatio, destinationRatio, getColourEncodingSystem(fps1000));
    var rX = scaledWidth / targetX;
    var rY = height / targetY;
    var newX;
    var newY;

    if (rX > rY)
    {
        // resize by X
        newX = targetX;
        newY = Math.round(height / rX);
    }
    else
    {
        // resize by Y
        newY = targetY;
        newX = Math.round(scaledWidth / rY);
    }

    // resize to multiple of 4
    newX -= newX % 4;
    newY -= newY % 4;

    if (newX != width || newY != height)
        app.video.addFilter("mpresize", "w=" + newX, "h=" + newY, "algo=0");

    // Black bars
    var barX = targetX - newX;
    var barY = targetY - newY;

    if (barX || barY)
        app.video.addFilter("addblack", "left=" + (barX >> 1), "right=" + (barX >> 1), "top=" + (barY >> 1), "bottom=" + (barY >> 1));
}