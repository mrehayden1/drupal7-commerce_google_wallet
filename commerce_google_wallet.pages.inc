<?php

/*
 *  Process a postback from Google Wallet
 */
function commerce_google_wallet_postback() {
  include_once 'libs/JWT.php';

  $settings = _commerce_google_wallet_settings();

  $postback_jwt = $_POST['jwt'];
  $postback_jwt_content = JWT::decode($postback_jwt, $settings['seller_secret']);

  // Log receipt of the postback
  $log_variables = array(
    '!postback' => '<pre>'.check_plain(var_export($postback_jwt_content, true)).'</pre>',
  );
  watchdog('commerce_google_wallet', 'Postback received: !postback', $log_variables);

  $seller_data = drupal_json_decode($postback_jwt_content->request->sellerData);

  $order_id = $seller_data['order_id'];
  if(!$order = commerce_order_load($order_id)) {
    watchdog('commerce_google_wallet', 'Failed to load Commerce order @commerce_order_id for Google Wallet order @order_no postback.', array('@commerce_order_id' => $order_id, '@order_no' => $postback_jwt_content->response->orderId));
    return;
  }

  $transaction = commerce_payment_transaction_new('google_wallet', $order_id);
  $transaction->instance_id = $order->data['payment_method'];
  $transaction->remote_id = $postback_jwt_content->response->orderId;
  $transaction->amount = commerce_currency_decimal_to_amount($postback_jwt_content->request->price, $postback_jwt_content->request->currencyCode);
  $transaction->currency_code = $postback_jwt_content->request->currencyCode;
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = t('Payment successful.');

  if(commerce_payment_transaction_save($transaction) == SAVED_NEW) {
    // Send a response to the postback request with the content of the 'response'
    // field else Google will not finalise the payment.
    echo $postback_jwt_content->response->orderId;     
  }
}
