/**
 * Attach handlers to toggle the twitter message field and inform the number
 * of characters remaining to achieve the max length
 */
(function ($) {
  Drupal.behaviors.twitter_post = {
    attach: function (context, settings) {
      $("#twitter-textfield", context).keyup(function() {
        var charsLeft = (140 - $(this).val().length);
        var descDiv = $(this).next();
        $(descDiv).html("<strong>" + charsLeft + "</strong> characters remaining");
        if (charsLeft < 0) {
          $(descDiv).addClass("negative");
        }
        else {
          $(descDiv).removeClass("negative");
        }
      });
    }
  };
}(jQuery));
