
public class Sensor {
	String lumi=null;
	String thermo=null;
	String id=null;
	
	public String getThermo() {
		return thermo;
	}

	public void setThermo(String thermo) {
		this.thermo = thermo;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getLumi() {
		return lumi;
	}
	
	public void setLumi(String lumi) {
		this.lumi = lumi;	
	}
	
	@Override
	public String toString() {
		return "Sensor [lumi=" + lumi + ", thermo=" + thermo + ", id=" + id + "]";
	}
	
}
