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
            component.addOperation("CreateShortcut", "@TargetDir@/OdroidFlashTool.exe", "@StartMenuDir@/Odroid Flash Tool.lnk",
             "workingDirectory=@TargetDir@", "@TargetDir@/images/Win32DiskImager.ico","iconId=2");
			 component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/Uninstall.lnk",
             "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll","iconId=2");
		}
		if(desktop)
		{
                     component.addOperation("CreateShortcut", "@TargetDir@/OdroidFlashTool.exe", "@DesktopDir@/Odroid Flash Tool.lnk",
             "workingDirectory=@TargetDir@", "@TargetDir@//images/Win32DiskImager.ico","iconId=2");
		}
     }
 }
 
 Component.prototype.loaded = function ()
{ // called after page is loaded
    var desktopIcon = component.userInterface( "ShortcutsForm" ).desktopIcon;
    //desktopIcon.
    console.log(desktopIcon.pixmap());
}
Component.prototype.shortcutsFormEntered = function ()
{
    var pageWidget = gui.pageWidgetByObjectName("ShortcutsForm");
}
