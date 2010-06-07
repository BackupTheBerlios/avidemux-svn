function videoAutoWizard(title, resolutions, codecs)
{
    var aspectRatios = [[1, 1], [4, 3], [16, 9]];
    var mnuResolution = new DFMenu(QT_TR_NOOP("Resolution:"));
    var mnuSourceRatio = new DFMenu(QT_TR_NOOP("Source Aspect Ratio:"));
    var mnuDestinationRatio = new DFMenu(QT_TR_NOOP("Destination Aspect Ratio:"));
    var dlgWizard = new DialogFactory(title);
    var i;

    if (resolutions != null)
    {
        for (i = 0; i < resolutions.length; i++)
            mnuResolution.addItem(resolutions[i][0].toString() + " x " + resolutions[i][1].toString());
    }

    for (i = 0; i < aspectRatios.length; i++)
    {
        mnuSourceRatio.addItem(aspectRatios[i][0].toString() + ":" + aspectRatios[i][1].toString());
        mnuDestinationRatio.addItem(aspectRatios[i][0].toString() + ":" + aspectRatios[i][1].toString());
    }
    
    if (codecs != null)
    {
        var mnuCodec = new DFMenu(QT_TR_NOOP("Codec:"));
               
        dlgWizard.addControl(mnuCodec);
        
        for (i = 0; i < codecs.length; i++)
            mnuCodec.addItem(codecs[i]);
    }

    if (resolutions != null)
        dlgWizard.addControl(mnuResolution);

    dlgWizard.addControl(mnuSourceRatio);
    dlgWizard.addControl(mnuDestinationRatio);

    if (dlgWizard.show())
        return [[resolutions == null ? -1 : resolutions[mnuResolution.index][0], resolutions == null ? -1 : resolutions[mnuResolution.index][1]],
            [aspectRatios[mnuSourceRatio.index][0], aspectRatios[mnuSourceRatio.index][1]],
            [aspectRatios[mnuDestinationRatio.index][0], aspectRatios[mnuDestinationRatio.index][1]], codecs == null ? -1 : mnuCodec.index];
    else
        return null;
}