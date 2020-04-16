package tools;

import java.awt.Desktop;
import java.net.URI;

public class BrowseWake {
	
	public void browse(String url) throws Exception {
        Desktop desktop = Desktop.getDesktop();
        if (Desktop.isDesktopSupported() && desktop.isSupported(Desktop.Action.BROWSE)) {
            URI uri = new URI(url);
            desktop.browse(uri);
        }
    }
}
