import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.dynamodb.DynamoDbClient;

public class main {
	public static void main(String[] args) {
		Dto dto = new Dto();
		String tableName = "DBDB_2";

		Region region = Region.AP_NORTHEAST_2;
		DynamoDbClient ddb = DynamoDbClient.builder()
                .region(region)
                .build();
		dto.describeDymamoDBTable(ddb, "DBDB_2");
		dto.listAllTables(ddb);
		String key = "";
		String value = "";
		dto.getDynamoDBItem(ddb, tableName, key,value); 
	}
}
