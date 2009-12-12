function videoAutoWizard(title, resolutions, codecs)
{
    var aspectRatios = [[1, 1], [4, 3], [16, 9]];
    var mnuResolution = new DFMenu("Resolution:");
    var mnuSourceRatio = new DFMenu("Source Aspect Ratio:");
    var mnuDestinationRatio = new DFMenu("Destination Aspect Ratio:");
    var dlgWizard = new DialogFactory(title);
    var i;

    for (i = 0; i < resolutions.length; i++)
        mnuResolution.addItem(resolutions[i][0].toString() + " x " + resolutions[i][1].toString());

    for (i = 0; i < aspectRatios.length; i++)
    {
        mnuSourceRatio.addItem(aspectRatios[i][0].toString() + ":" + aspectRatios[i][1].toString());
        mnuDestinationRatio.addItem(aspectRatios[i][0].toString() + ":" + aspectRatios[i][1].toString());
    }
    
    if (codecs != null)
    {
        var mnuCodec = new DFMenu("Codec:");
               
        dlgWizard.addControl(mnuCodec);
        
        for (i = 0; i < codecs.length; i++)
            mnuCodec.addItem(codecs[i]);
    }

    dlgWizard.addControl(mnuResolution);
    dlgWizard.addControl(mnuSourceRatio);
    dlgWizard.addControl(mnuDestinationRatio);

    if (dlgWizard.show())
        return [[resolutions[mnuResolution.index][0], resolutions[mnuResolution.index][1]],
            [aspectRatios[mnuSourceRatio.index][0], aspectRatios[mnuSourceRatio.index][1]],
            [aspectRatios[mnuDestinationRatio.index][0], aspectRatios[mnuDestinationRatio.index][1]], codecs == null ? -1 : mnuCodec.index];
    else
        return null;
}