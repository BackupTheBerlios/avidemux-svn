function rescaleVideoDimension(value, sourceRatio, destinationRatio, encodingSystem)
{
    return Math.round(value * getPixelAspectRatio(sourceRatio, encodingSystem) / getPixelAspectRatio(destinationRatio, encodingSystem));
}

function getPixelAspectRatio(ratio, encodingSystem)
{
    if (ratio == "4:3")
    {
        if (encodingSystem == "PAL")
            return 16 / 15;
        else if (encodingSystem == "NTSC" || encodingSystem == "FILM")
            return 8 / 9;
    }
    else if (ratio == "16:9")
    {
        if (encodingSystem == "PAL")
            return 64 / 45;
        else if (encodingSystem == "NTSC" || encodingSystem == "FILM")
            return 32 / 27;
    }

    return 1;
}

function getColourEncodingSystem(fps1000)
{
    var encodingSystem = "Unknown";

    if (fps1000 > 24700 && fps1000 < 25300)
        encodingSystem = "PAL";
    else if (fps1000 > 23676 && fps1000 < 24276)
        encodingSystem = "FILM";
    else if (fps1000 > 29670 && fps1000 < 30270)
        encodingSystem = "NTSC";

    return encodingSystem;
}

function getVideoProperties(video)
{
    var filters = video.appliedFilters;
    var width, height, fps1000;

    if (filters.length > 0)
    {
        var lastFilter = video.appliedFilters[filters.length - 1];

        width = lastFilter.width;
        height = lastFilter.height;
        fps1000 = lastFilter.fps1000;
    }
    else
    {
        width = video.width;
        height = video.height;
        fps1000 = video.fps1000;
    }

    return [width, height, fps1000];
}