
import java.awt.print.PrinterAbortException;
import java.io.Console;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import com.google.gson.Gson;
import com.google.gson.JsonArray;

import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.dynamodb.DynamoDbClient;

/**
 * Servlet implementation class Status
 */
@WebServlet("/status")
public class Status extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Status() {
        super();
        // TODO Auto-generated constructor stub
    }

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String tableName = "IOT_DB2";
		Region region = Region.US_EAST_2;
		DynamoDbClient ddb = DynamoDbClient.builder()
                .region(region)
                .build();
		String key = "number";
		JsonArray jsonArray = new JsonArray();
		String[] valuesArrayList = {"1","2","3"};
		for(String value:valuesArrayList) {
			JsonArray data = Dto.getJsonDynamoDBItem(ddb, tableName, key, value); 
			jsonArray.addAll(data);
		}
		response.getWriter().write(jsonArray.toString());
	}
}
