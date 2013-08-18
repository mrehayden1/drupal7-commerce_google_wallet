Description
===========
Integrates Commerce and Google Wallet as an offsite payment method for products and subscriptions. Doesn't actually redirect but uses the GW client side JavaScript app to take payments through your customer's GW account.

Dependencies
============
* Drupal 7 Core modules
* Commerce
* Commerce Customizable Products

Usage
=====
* Set three variables:google_wallet_[seller_identifier | seller_secret | sandbox_mode]
* All the usual stuff for Drupal Commerce payment methods
* Wash and go

For Subscriptions
-----------------
* Enable subscriptions for a product (product types form)
* Enable subscriptions for a line item (line-item types form)
* Integrate the two where needed, i.e. choose the line item type for add to cart forms for the products.

Notes
=====
* Lumps all produts into a single GW transaction. You could create a single product checkout module if it really bothers you. You could even disable the cart page for even less confusion.
* Doesn't process postbacks for cancelled subscriptions or recurring payments yet.
* There is no API to cancel subscriptions. Customers must do this through their own GW account.
