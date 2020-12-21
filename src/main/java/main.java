import java.util.Map;

import org.json.simple.JSONArray;

import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.dynamodb.DynamoDbClient;
import software.amazon.awssdk.services.dynamodb.model.AttributeValue;

public class main {
	public static void main(String[] args) {
		Dto dto = new Dto();
		String tableName = "DBDB_2";

		Region region = Region.AP_NORTHEAST_2;
		DynamoDbClient ddb = DynamoDbClient.builder()
                .region(region)
                .build();
		dto.describeDymamoDBTable(ddb, tableName);
		dto.listAllTables(ddb);
		String key = "";
		String value = "";
		JSONArray data = dto.getJsonDynamoDBItem(ddb, tableName, key,value); 
		// return jsonarray
	}
}
