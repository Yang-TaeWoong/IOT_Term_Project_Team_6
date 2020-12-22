import java.io.PrintWriter;

import org.json.simple.JSONArray;

import com.google.gson.JsonArray;

import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.dynamodb.DynamoDbClient;

public class main {
	public static void main(String[] args){
		
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
		System.out.print(jsonArray);
	}
}
