import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.dynamodb.model.DynamoDbException;
import software.amazon.awssdk.services.dynamodb.model.ListTablesRequest;
import software.amazon.awssdk.services.dynamodb.model.ListTablesResponse;
import software.amazon.awssdk.services.dynamodb.DynamoDbClient;
import software.amazon.awssdk.services.dynamodb.model.AttributeDefinition;
import software.amazon.awssdk.services.dynamodb.model.DescribeTableRequest;
import software.amazon.awssdk.services.dynamodb.model.ProvisionedThroughputDescription;
import software.amazon.awssdk.services.dynamodb.model.TableDescription;
import java.util.List;
import software.amazon.awssdk.services.dynamodb.model.AttributeValue;
import software.amazon.awssdk.services.dynamodb.model.GetItemRequest;
import java.util.HashMap;
import java.time.Instant;
import java.util.Iterator;
import software.amazon.awssdk.enhanced.dynamodb.DynamoDbEnhancedClient;
import software.amazon.awssdk.enhanced.dynamodb.DynamoDbTable;
import software.amazon.awssdk.enhanced.dynamodb.TableSchema;
import software.amazon.awssdk.enhanced.dynamodb.mapper.annotations.DynamoDbBean;
import software.amazon.awssdk.enhanced.dynamodb.mapper.annotations.DynamoDbPartitionKey;
import software.amazon.awssdk.enhanced.dynamodb.mapper.annotations.DynamoDbSortKey;


import java.util.Map;
import java.util.Set;


public class Dto {
	
	public static void listAllTables(DynamoDbClient ddb){

	    boolean moreTables = true;
	    String lastName = null;

	    while(moreTables) {
	        try {
	            ListTablesResponse response = null;
	            if (lastName == null) {
	                ListTablesRequest request = ListTablesRequest.builder().build();
	                response = ddb.listTables(request);
	            } else {
	                ListTablesRequest request = ListTablesRequest.builder()
	                        .exclusiveStartTableName(lastName).build();
	                response = ddb.listTables(request);
	            }

	            List<String> tableNames = response.tableNames();

	            if (tableNames.size() > 0) {
	                for (String curName : tableNames) {
	                    System.out.format("* %s\n", curName);
	                }
	            } else {
	                System.out.println("No tables found!");
	                System.exit(0);
	            }

	            lastName = response.lastEvaluatedTableName();
	            if (lastName == null) {
	                moreTables = false;
	            }
	        } catch (DynamoDbException e) {
	            System.err.println(e.getMessage());
	            System.exit(1);
	        }
	    }
	}

	public static void describeDymamoDBTable(DynamoDbClient ddb,String tableName ) {

	    DescribeTableRequest request = DescribeTableRequest.builder()
	            .tableName(tableName)
	            .build();
	    try {
	        TableDescription tableInfo =
	                ddb.describeTable(request).table();

	        if (tableInfo != null) {
	            System.out.format("Table name  : %s\n",
	                    tableInfo.tableName());
	            System.out.format("Table ARN   : %s\n",
	                    tableInfo.tableArn());
	            System.out.format("Status      : %s\n",
	                    tableInfo.tableStatus());
	            System.out.format("Item count  : %d\n",
	                    tableInfo.itemCount().longValue());
	            System.out.format("Size (bytes): %d\n",
	                    tableInfo.tableSizeBytes().longValue());

	            ProvisionedThroughputDescription throughputInfo =
	                    tableInfo.provisionedThroughput();
	            System.out.println("Throughput");
	            System.out.format("  Read Capacity : %d\n",
	                    throughputInfo.readCapacityUnits().longValue());
	            System.out.format("  Write Capacity: %d\n",
	                    throughputInfo.writeCapacityUnits().longValue());

	            List<AttributeDefinition> attributes =
	                    tableInfo.attributeDefinitions();
	            System.out.println("Attributes");

	            for (AttributeDefinition a : attributes) {
	                System.out.format("  %s (%s)\n",
	                        a.attributeName(), a.attributeType());
	            }
	        }
	    } catch (DynamoDbException e) {
	        System.err.println(e.getMessage());
	        System.exit(1);
	    }
	}
	public static void getDynamoDBItem(DynamoDbClient ddb,String tableName,String key,String keyVal ) {

	    HashMap<String,AttributeValue> keyToGet = new HashMap<String,AttributeValue>();

	    keyToGet.put(key, AttributeValue.builder()
	            .s(keyVal).build());

	    GetItemRequest request = GetItemRequest.builder()
	            .key(keyToGet)
	            .tableName(tableName)
	            .build();

	    try {
	        Map<String,AttributeValue> returnedItem = ddb.getItem(request).item();

	        if (returnedItem != null) {
	            Set<String> keys = returnedItem.keySet();
	            System.out.println("Amazon DynamoDB table attributes: \n");

	            for (String key1 : keys) {
	                System.out.format("%s: %s\n", key1, returnedItem.get(key1).toString());
	            }
	        } else {
	            System.out.format("No item found with the key %s!\n", key);
	        }
	    } catch (DynamoDbException e) {
	        System.err.println(e.getMessage());
	        System.exit(1);
	    }
	}
	public static void scan( DynamoDbEnhancedClient enhancedClient) {

	    try{
	        // Create a DynamoDbTable object
	        DynamoDbTable<Sensor> custTable = enhancedClient.table("DBDB_2", TableSchema.fromBean(Sensor.class));
	        Iterator<Sensor> results = custTable.scan().items().iterator();
	        while (results.hasNext()) {

	        	Sensor rec = results.next();
	            System.out.println("The record id is "+rec.getLumi());
	        }

	    } catch (DynamoDbException e) {
	        System.err.println(e.getMessage());
	        System.exit(1);
	    }
	    System.out.println("Done");
	}

}
