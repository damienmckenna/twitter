Introduction
============
Twitter module allows listing tweets in blocks or pages. Its integration with
Views opens the door to all sorts of formatting (ie. as an automatic slideshow
with views_slideshow). It also provides useful input filters to easily link
Twitter accounts and searches within text.

Twitter's submodules allow posting to twitter, executing actions/rules when
tweeting or login with a Twitter account.

Installation
============
The OAuth module is required:
  http://drupal.org/project/oauth

Note: only the v6.x-3.x releases of OAuth have been tested, the older v6.x-2.x
releases may not work.

When installing the Twitter module without the above being available, Drupal
will complain about a missing "oauth_common" module. This module is actually
provided by the OAuth project - the module name and project name are not the
same, hence this error.

Once OAuth and Twitter have been enabled, go to admin/settings/twitter and
follow the instructions in order to provide the Twitter Application keys.

Further installation instructions can be found at:
  http://drupal.org/node/1346824

How to use the username and hashtag input filters
=================================================
1. Go to admin/settings/filters.
2. Select the text format where you want to use the filters.
3. At "Enabled filters" check the Twitter converters.

After that, clear cache and try to create a page with the following body:

#drupal @drupal

The above links to a search in Twitter over the #drupal tag and a to the @drupal
profile. These filters are avilable when configuring a tweets Views.

How to post to Twitter
======================
1. Read the OAuth section to install and configure OAuth.
2. Once OAuth has been configured, go to admin/settings/twitter/post and select
   from which node types a user may post to Twitter and the default message.
3. Verify permissions at admin/user/permissions.
4. Add a Twitter account and try to edit or post content.

Further information can be found at http://drupal.org/node/1016584.

How to sign in with Twitter
===========================
Existing and new users can sign in with Twitter by enabling the twitter_signin
module. The following scenarios are being contemplated so far:

* A visitor logs in with his Twitter account and, once authenticated at
  Twitter.com, fills in their email in the Drupal registration form and receives
  an email to log in and set their account password.
* An existing user signs in with Twitter and then logs in into their Drupal user
  account. This results in the Twitter account getting related to the user
  account so next time Twitter sign in will work.
* An existing user with an already configured Twitter account can log in
  automatically by clicking on the Sign in with Twitter button.
