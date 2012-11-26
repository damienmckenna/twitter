Introduction
============
Twitter module allows listing tweets in blocks or pages. Its integration with Views opens the
door to all sorts of formatting (ie. as an automatic slideshow with views_slideshow). It also
provides useful input filters to easily link Twitter accounts and searches within text.

Twitter's submodules allow posting to twitter, executing actions/rules when tweeting or login
with a Twitter account.

Installation
============
Twitter's REST API library is out of the module. Although this needs an extra step on installation,
it makes the library reusable for both Drupal 6 and 7 versions and also other modules.

To set it up, just download twitter.lib.php from https://github.com/juampy72/twitter-rest-php/downloads
to your libraries directory (normally sites/all/libraries) so its final location is
sites/all/libraries/twitter/twitter.lib.php

To verify that it has been installed correctly, view the Status Report at
http://yoursite/admin/reports/status.

OAuth
=====
OAuth module is required for all requests to the Twitter REST API 1.1.

When you download the OAuth module, get the latest stable release available at http://drupal.org/project/oauth

Once OAuth has been enabled, go to admin/settings/twitter and follow instructions in order
to provide your Application keys.

How to add a Twitter account to a Drupal user account
=====================================================
Read http://drupal.org/node/1253026 for details.

How to use the username and hashtag input filters
=================================================
1. Go to admin/settings/filters.
2. Select the input format where you want to use the filters.
3. At "Enabled filters" check the Twitter converters.

After that, clear cache and try to create a page with the following body:

#drupal @drupal

The above links to a search in Twitter over the #drupal tag and a to the @drupal profile.
These filters are avilable when configuring a tweets Views.

How to post to Twitter
======================
1. Enable the twitter_post submodule.
2. Make sure that your Twitter application at http://dev.twitter.com has read/write
   permissions.
3. Set up the OAuth section at admin/settings/twitter and add a Twitter account.
4. Go to admin/settings/twitter/post and select from which
   node types a user may post to Twitter and the default message.
5. Verify permissions at admin/user/permissions#module-twitter_post.
6. Edit or post new content and test the Post to Twitter field.

You can find further info at http://drupal.org/node/1016584.

How to sign in with Twitter
===========================
Existing and new users can sign in with Twitter by enabling the twitter_signin module.
The following scenarios are being contemplated so far:

* A visitor logs in with his Twitter account and, once authenticated at Twitter.com,
  she fills in his email in the Drupal registration form and receives an email to log
  in and set her account password.
* An existing user signs in with Twitter and then logs in into her Drupal user account.
  This results in the Twitter account getting related to the user account so next time
  Twitter sign in will work.
* An existing user with an already configured Twitter account can log in automatically
  by clicking on the Sign in with Twitter button.
