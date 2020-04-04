Feature: Consumer dafka protocol

  Scenario: First record for topic with offset reset earliest 

    Given a dafka consumer subscribed to topic 'hello'
    When a RECORD message with sequence larger 0 is sent on topic 'hello'
    Then the consumer will send a FETCH message for the topic 'hello'
    When a RECORD message with sequence 0 and content 'CONTENT' is send on topic 'hello'
    Then a consumer_msg is sent to the user with topic 'hello' and content 'CONTENT'
