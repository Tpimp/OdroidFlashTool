function Component()
{
    // constructor
    component.loaded.connect(this, Component.prototype.loaded);
    if (!installer.addWizardPage(component, "ShortcutsForm", QInstaller.ReadyForInstallation))
        console.log("Could not add the dynamic page.");
}



Component.prototype.createOperations = function()
{
    component.createOperations();
    var startmenu = component.userInterface( "ShortcutsForm" ).startMenuShortcut.checked;
    var desktop = component.userInterface( "ShortcutsForm" ).desktopShortcut.checked;
    if (systemInfo.productType === "windows") {
		if(startmenu)
		{
            component.addOperation("CreateShortcut", "@TargetDir@/Chessgames.exe", "@StartMenuDir@/ChessGames.lnk",
             "workingDirectory=@TargetDir@", "@TargetDir@/play-zone.ico","iconId=2");
			 component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/Uninstall.lnk",
             "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll","iconId=2");
		}
		if(desktop)
		{
		     component.addOperation("CreateShortcut", "@TargetDir@/Chessgames.exe", "@DesktopDir@/ChessGames.lnk",
             "workingDirectory=@TargetDir@", "@TargetDir@/play-zone.ico","iconId=2");
		}
     }
 }
 
 Component.prototype.loaded = function ()
{
    if (pageWidget != null) {
        console.log("Setting the widgets label text.")
    }
}
Component.prototype.shortcutsFormEntered = function ()
{
    var pageWidget = gui.pageWidgetByObjectName("ShortcutsForm");
}